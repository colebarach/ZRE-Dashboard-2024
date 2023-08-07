// Header
#include "can_socket.h"

// C++ Libraries
#include <string>
#include <iostream>

// C Standard Libraries
#include <errno.h>
#include <time.h>
#include <string.h>

// POSIX Libraries
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

// SocketCAN Libraries
#include <linux/can.h>
#include <linux/can/raw.h>

namespace Network
{
    CanSocket::CanSocket(const char* deviceName)
    {
        #ifdef DEBUG_GENERAL
        std::cout << "Creating CAN socket bound to device \"" << deviceName << "\"... ";
        #endif

        // Create the socket using the CAN protocol family and the raw CAN protol
        this->descriptor = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if(this->descriptor == -1)
        {
            this->errorCode = errno;
            throw std::runtime_error(std::string("Failed to create CAN socket for interface \"") + deviceName + "\": " + strerror(this->errorCode));
        }

        // Create the interface structure
        struct ifreq interface;
        
        // Set interface name, set terminating character if name exceeds bounds
        strncpy(interface.ifr_name, deviceName, IFNAMSIZ - 1);
        interface.ifr_name[IFNAMSIZ - 1] = '\0';

        // Get the interface index from its name
        this->errorCode = ioctl(this->descriptor, SIOCGIFINDEX, &interface);
        if(this->errorCode == -1)
        {
            this->errorCode = errno;
            throw std::runtime_error(std::string("Failed to get index of CAN interface \"") + deviceName + "\": " + std::string(strerror(this->errorCode)));
        }

        // Allocate the CAN socket address
        struct sockaddr_can address =
        {
            .can_family  = AF_CAN,                 // CAN Address family
            .can_ifindex = interface.ifr_ifindex,  // CAN Interface index
        };

        // Bind the socket to the specified address
        this->errorCode = bind(this->descriptor, reinterpret_cast<sockaddr*>(&address), static_cast<socklen_t>(sizeof(address)));
        if(this->errorCode == -1)
        {
            this->errorCode = errno;
            throw std::runtime_error(std::string("Failed bind CAN socket at interface \"") + deviceName + "\" to index \"" + std::to_string(interface.ifr_ifindex) + "\": " + std::string(strerror(this->errorCode)));
        }

        #ifdef DEBUG_GENERAL
        std::cout << "Socket created." << std::endl;
        #endif
    }

    void CanSocket::sendMessage(const uint64_t* data, const uint8_t* dataLength, const uint16_t* id)
    {
        #ifdef DEBUG_TRAFFIC
        std::cout << "Sending CAN message: { ID: " << *id << ", Length: " << *dataLength << ", Data: " << std::hex << *data << std::dec << "} ";
        #endif

        // Validate data count
        if(*dataLength > 8U) throw std::invalid_argument("Failed to send message: Data length cannot be greater than 8 bytes.");

        // Allocate message frame
        struct can_frame messageFrame =
        {
            .can_id = static_cast<canid_t>(*id),
            .len    = static_cast<__u8>(*dataLength),
        };

        // Copy message data
        for(uint8_t index = 0; index < *dataLength; ++index)
        {
            messageFrame.data[index] = (*data >> (index * 8)) & 0xFF;
        }

        // Send message frame
        this->errorCode = write(this->descriptor, &messageFrame, sizeof(struct can_frame));
        if(this->errorCode < static_cast<long long int>(sizeof(struct can_frame)))
        {
            this->errorCode = errno;

            // Check for timeout
            if(this->errorCode == EAGAIN || this->errorCode == EWOULDBLOCK || this->errorCode == EINPROGRESS)
            {
                throw Timeout();
            }
            
            throw std::runtime_error("Failed to send CAN message: " + std::string(strerror(this->errorCode)));
        }

        #ifdef DEBUG_TRAFFIC
        std::cout << "Sent." << std::endl;
        #endif
    }

    void CanSocket::readMessage(uint64_t* data, uint8_t* dataLength, uint16_t* id)
    {
        #ifdef DEBUG_TRAFFIC
        std::cout << "Reading CAN message... ";
        #endif

        // Allocate message frame
        struct can_frame messageFrame;

        // Read message
        this->errorCode = read(this->descriptor, &messageFrame, sizeof(struct can_frame));

        if(this->errorCode < static_cast<long long int>(sizeof(struct can_frame)))
        {
            this->errorCode = errno;

            // Check for timeout
            if(this->errorCode == EAGAIN || this->errorCode == EWOULDBLOCK || this->errorCode == EINPROGRESS)
            {
                throw Timeout();
            }

            throw std::runtime_error("Failed to read CAN message: " + std::string(strerror(this->errorCode)));
        }

        // Read metadata
        *id         = static_cast<uint16_t>(messageFrame.can_id);
        *dataLength = static_cast<uint8_t>(messageFrame.len);

        // Validate data count
        if(*dataLength > 8) throw std::runtime_error("Failed to read CAN message: Invalid data count.");

        // Copy message data
        *data = 0;
        for(uint8_t index = 0; index < *dataLength; ++index)
        {
            *data |= static_cast<uint64_t>(messageFrame.data[index]) << (index * 8);
        }

        #ifdef DEBUG_TRAFFIC
        std::cout << "Read CAN message: { ID: " << *id << ", Length: " << *dataLength << ", Data: " << std::hex << *data << std::dec << "}" << std::endl;
        #endif
    }

    uint64_t CanSocket::parseUnsignedInt(const uint64_t& data, const CanSignal& signal)
    {
        #ifdef DEBUG_PARSE
        std::cout << "Parsing signal \"" << signal.name << "\"... ";
        #endif

        #ifdef DEBUG_DATATYPE_WARNING
        switch(signal.datatypeId)
        {
            case ID_DATATYPE_UINT:
                break;
            case ID_DATATYPE_INT:
                std::cout << "Warning: Interpreting integer signal \"" << signal.name << "\" as unsigned integer." << std::endl;
                break;
            case ID_DATATYPE_DOUBLE:
                std::cout << "Warning: Interpreting double signal \"" << signal.name << "\" as unsigned integer." << std::endl;
                break;
            case ID_DATATYPE_BOOL:
                std::cout << "Warning: Interpreting boolean signal \"" << signal.name << "\" as unsigned integer." << std::endl;
                break;
            default:
                std::cout << "Warning: Interpreting unknown signal \"" << signal.name << "\" as unsigned integer." << std::endl;
                break;
        }
        #endif

        uint64_t dataBuffer = (data >> signal.bitPosition) & signal.bitMask;
        // dataBuffer = static_cast<double>(dataBuffer) * signal.scaleFactor + signal.offset; // TODO: Scale offset

        #ifdef DEBUG_PARSE
        std::cout << "Parsed " << dataBuffer << "." << std::endl;
        #endif

        return dataBuffer;
    }

    int64_t CanSocket::parseSignedInt(const uint64_t& data, const CanSignal& signal)
    {
        #ifdef DEBUG_PARSE
        std::cout << "Parsing signal \"" << signal.name << "\"... ";
        #endif

        #ifdef DEBUG_DATATYPE_WARNING
        switch(signal.datatypeId)
        {
            case ID_DATATYPE_INT:
                break;
            case ID_DATATYPE_UINT:
                std::cout << "Warning: Interpreting unsigned integer signal \"" << signal.name << "\" as integer." << std::endl;
                break;
            case ID_DATATYPE_DOUBLE:
                std::cout << "Warning: Interpreting double signal \"" << signal.name << "\" as integer." << std::endl;
                break;
            case ID_DATATYPE_BOOL:
                std::cout << "Warning: Interpreting boolean signal \"" << signal.name << "\" as integer." << std::endl;
                break;
            default:
                std::cout << "Warning: Interpreting unknown signal \"" << signal.name << "\" as integer." << std::endl;
                break;
        }
        #endif

        int64_t dataBuffer = (data >> signal.bitPosition) & signal.bitMask;
        // dataBuffer = static_cast<double>(dataBuffer) * signal.scaleFactor + signal.offset; // TODO: OFFSET AND SCALE ARE IGNORED

        #ifdef DEBUG_PARSE
        std::cout << "Parsed " << dataBuffer << "." << std::endl;
        #endif

        return dataBuffer;
    }

    double CanSocket::parseDouble(const uint64_t& data, const CanSignal& signal)
    {
        std::cout << "WARNING: Parsing doubles is not implemented!!!" << std::endl;
        return 0;
    }

    bool CanSocket::parseBool(const uint64_t& data, const CanSignal& signal)
    {
        #ifdef DEBUG_PARSE
        std::cout << "Parsing signal \"" << signal.name << "\"... ";
        #endif

        #ifdef DEBUG_DATATYPE_WARNING
        switch(signal.datatypeId)
        {
            case ID_DATATYPE_BOOL:
                break;
            case ID_DATATYPE_UINT:
                std::cout << "Warning: Interpreting unsigned integer signal \"" << signal.name << "\" as boolean." << std::endl;
                break;
            case ID_DATATYPE_INT:
                std::cout << "Warning: Interpreting integer signal \"" << signal.name << "\" as boolean." << std::endl;
                break;
            case ID_DATATYPE_DOUBLE:
                std::cout << "Warning: Interpreting double signal \"" << signal.name << "\" as boolean." << std::endl;
                break;
            default:
                std::cout << "Warning: Interpreting unknown signal \"" << signal.name << "\" as boolean." << std::endl;
                break;
        }
        #endif

        uint64_t dataBuffer = (data >> signal.bitPosition) & signal.bitMask;

        #ifdef DEBUG_PARSE
        std::cout << "Parsed " << (dataBuffer == 1) << "." << std::endl;
        #endif

        return dataBuffer == 1;
    }

    uint64_t CanSocket::encodeUnsignedInt(const uint64_t& data, const CanSignal& signal)
    {
        #ifdef DEBUG_PARSE
        std::cout << "Encoding signal \"" << signal.name << "\": " << data << "... ";
        #endif

        #ifdef DEBUG_DATATYPE_WARNING
        switch(signal.datatypeId)
        {
            case ID_DATATYPE_UINT:
                break;
            case ID_DATATYPE_INT:
                std::cout << "Warning: Encoding integer signal \"" << signal.name << "\" as unsigned integer." << std::endl;
                break;
            case ID_DATATYPE_DOUBLE:
                std::cout << "Warning: Encoding double signal \"" << signal.name << "\" as unsigned integer." << std::endl;
                break;
            case ID_DATATYPE_BOOL:
                std::cout << "Warning: Encoding boolean signal \"" << signal.name << "\" as unsigned integer." << std::endl;
                break;
            default:
                std::cout << "Warning: Encoding unknown signal \"" << signal.name << "\" as unsigned integer." << std::endl;
                break;
        }
        #endif

        // TODO: Scale offset
        uint64_t dataBuffer = (data & signal.bitMask) << signal.bitPosition;

        #ifdef DEBUG_PARSE
        std::cout << "Encoded " << dataBuffer << "." << std::endl;
        #endif

        return dataBuffer;
    }

    uint64_t CanSocket::encodeSignedInt(const int64_t& data, const CanSignal& signal)
    {
        #ifdef DEBUG_PARSE
        std::cout << "Encoding signal \"" << signal.name << "\": " << data << "... ";
        #endif

        #ifdef DEBUG_DATATYPE_WARNING
        switch(signal.datatypeId)
        {
            case ID_DATATYPE_INT:
                break;
            case ID_DATATYPE_UINT:
                std::cout << "Warning: Encoding unsigned integer signal \"" << signal.name << "\" as integer." << std::endl;
                break;
            case ID_DATATYPE_DOUBLE:
                std::cout << "Warning: Encoding double signal \"" << signal.name << "\" as integer." << std::endl;
                break;
            case ID_DATATYPE_BOOL:
                std::cout << "Warning: Encoding boolean signal \"" << signal.name << "\" as integer." << std::endl;
                break;
            default:
                std::cout << "Warning: Encoding unknown signal \"" << signal.name << "\" as integer." << std::endl;
                break;
        }
        #endif

        uint64_t dataBuffer = (data & signal.bitMask) << signal.bitPosition;
        
        #ifdef DEBUG_PARSE
        std::cout << "Encoded " << dataBuffer << "." << std::endl;
        #endif

        return dataBuffer;
    }

    uint64_t CanSocket::encodeDouble(const double& data, const CanSignal& signal)
    {
        std::cout << "WARNING: Encoding doubles is not implemented!!!" << std::endl;
        return 0;
    }

    uint64_t CanSocket::encodeBool(const bool& data, const CanSignal& signal)
    {
        #ifdef DEBUG_PARSE
        std::cout << "Encoding signal \"" << signal.name << "\": " << data << "... ";
        #endif

        #ifdef DEBUG_DATATYPE_WARNING
        switch(signal.datatypeId)
        {
            case ID_DATATYPE_BOOL:
                break;
            case ID_DATATYPE_UINT:
                std::cout << "Warning: Encoding unsigned integer signal \"" << signal.name << "\" as boolean." << std::endl;
                break;
            case ID_DATATYPE_INT:
                std::cout << "Warning: Encoding integer signal \"" << signal.name << "\" as boolean." << std::endl;
                break;
            case ID_DATATYPE_DOUBLE:
                std::cout << "Warning: Encoding double signal \"" << signal.name << "\" as boolean." << std::endl;
                break;
            default:
                std::cout << "Warning: Encoding unknown signal \"" << signal.name << "\" as boolean." << std::endl;
                break;
        }
        #endif

        uint64_t dataBuffer = (data & signal.bitMask) << signal.bitPosition;

        #ifdef DEBUG_PARSE
        std::cout << "Encoded " << dataBuffer << "." << std::endl;
        #endif

        return dataBuffer;
    }

    void CanSocket::reallocateMessages(CanSignal** signalArray, CanMessage** messageArray, size_t oldSignalCount, size_t newSignalCount, size_t oldMessageCount, size_t newMessageCount)
    {
        #ifdef DEBUG_GENERAL
        std::cout << "Reallocating messages. Signal count: " << oldSignalCount << " => " << newSignalCount << ", Message count: " << oldMessageCount << " => " << newMessageCount << "... ";
        #endif

        // Validate parameters
        if(newSignalCount >= oldSignalCount || newMessageCount >= oldMessageCount)
        {
            throw std::runtime_error("Failed to reallocate CAN message array: The new size must be less than the old size.");
        }

        // Create new arrays
        CanSignal*  newSignalArray  = new CanSignal[newSignalCount];
        CanMessage* newMessageArray = new CanMessage[newMessageCount];

        // Copy message values
        for(size_t index = 0; index < newMessageCount; ++index)
        {
            newMessageArray[index].signalArray = newSignalArray;
            newMessageArray[index].signalIndex = (*messageArray)[index].signalIndex;
            newMessageArray[index].signalCount = (*messageArray)[index].signalCount;
            newMessageArray[index].name        = (*messageArray)[index].name;
            newMessageArray[index].id          = (*messageArray)[index].id;
        }

        // Copy message values
        for(size_t index = 0; index < newSignalCount; ++index)
        {
            newSignalArray[index].messageArray = newMessageArray;
            newSignalArray[index].messageIndex = (*signalArray)[index].messageIndex;
            newSignalArray[index].name         = (*signalArray)[index].name;
            newSignalArray[index].bitPosition  = (*signalArray)[index].bitPosition;
            newSignalArray[index].bitLength    = (*signalArray)[index].bitLength;
            newSignalArray[index].bitMask      = (*signalArray)[index].bitMask;
            newSignalArray[index].isSigned     = (*signalArray)[index].isSigned;
        }

        // Delete old arrays
        delete [] *signalArray;
        delete [] *messageArray;

        // Update references
        *signalArray  = newSignalArray;
        *messageArray = newMessageArray;

        #ifdef DEBUG_GENERAL
        std::cout << "Reallocated." << std::endl;
        #endif
    }
}
