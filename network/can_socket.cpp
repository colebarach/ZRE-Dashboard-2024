// Header
#include "can_socket.h"

// Includes
#include "log.h"

// C Standard Libraries
#include <errno.h>
#include <time.h>
#include <string.h>
#include <math.h>

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
        LOG_INFO("Creating CAN socket bound to device '%s'...\n", deviceName);

        // Create the socket using the CAN protocol family and the raw CAN protol
        this->descriptor = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if(this->descriptor == -1)
        {
            this->errorCode = errno;
            LOG_ERROR("Failed to create CAN socket for device '%s': %s\n", deviceName, strerror(this->errorCode));
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
            LOG_ERROR("Failed to get index of CAN device '%s': %s\n", deviceName, strerror(this->errorCode));
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
            LOG_ERROR("Failed to bind CAN socket '%s' to index %i: %s\n", deviceName, interface.ifr_ifindex, strerror(this->errorCode));
            throw std::runtime_error(std::string("Failed bind CAN socket '") + deviceName + "' to index " + std::to_string(interface.ifr_ifindex) + ": " + std::string(strerror(this->errorCode)));
        }

        LOG_INFO("CAN socket created.\n");
    }

    void CanSocket::sendMessage(const uint64_t* data, const uint8_t* dataLength, const uint16_t* id)
    {
        // Validate data count
        if(*dataLength > 8U)
        {
            LOG_ERROR("Failed to send message: Data length cannot be greater than 8 bytes.\n");
            throw std::invalid_argument("Failed to send message: Data length cannot be greater than 8 bytes.\n");
        }

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
            
            LOG_ERROR("Failed to send CAN message: %s\n", strerror(this->errorCode));
            throw std::runtime_error("Failed to send CAN message: " + std::string(strerror(this->errorCode)));
        }
    }

    void CanSocket::readMessage(uint64_t* data, uint8_t* dataLength, uint16_t* id)
    {
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

            LOG_ERROR("Failed to read CAN message: %s\n", strerror(this->errorCode));
            throw std::runtime_error("Failed to read CAN message: " + std::string(strerror(this->errorCode)));
        }

        // Read metadata
        *id         = static_cast<uint16_t>(messageFrame.can_id);
        *dataLength = static_cast<uint8_t>(messageFrame.len);

        // Validate data count
        if(*dataLength > 8)
        {
            LOG_ERROR("Failed to read CAN message: Invalid data count.\n");
            throw std::runtime_error("Failed to read CAN message: Invalid data count.");
        }

        // Copy message data
        *data = 0;
        for(uint8_t index = 0; index < *dataLength; ++index)
        {
            *data |= static_cast<uint64_t>(messageFrame.data[index]) << (index * 8);
        }
    }

    uint64_t CanSocket::decodeUnsignedInt(const uint64_t& data, const CanSignal& signal)
    {
        // Validate datatype
        switch(signal.datatypeId)
        {
        case ID_DATATYPE_UINT:
            break;
        case ID_DATATYPE_INT:
            LOG_WARN("Decoding integer signal '%s' as unsigned integer.\n", signal.name);
            break;
        case ID_DATATYPE_DOUBLE:
            LOG_WARN("Decoding double signal '%s' as unsigned integer.\n", signal.name);
            break;
        case ID_DATATYPE_BOOL:
            LOG_WARN("Decoding boolean signal '%s' as unsigned integer.\n", signal.name);
            break;
        default:
            LOG_WARN("Decoding unknown signal '%s' as unsigned integer.\n", signal.name);
            break;
        }
        
        // Mask integer
        uint64_t dataBuffer = (data >> signal.bitPosition) & signal.bitMask;
        
        // Scale factor & offset
        dataBuffer = static_cast<uint64_t>(dataBuffer * signal.scaleFactor + signal.offset);

        return dataBuffer;
    }

    int64_t CanSocket::decodeSignedInt(const uint64_t& data, const CanSignal& signal)
    {
        switch(signal.datatypeId)
        {
        case ID_DATATYPE_INT:
            break;
        case ID_DATATYPE_UINT:
            LOG_WARN("Decoding unsigned integer signal '%s' as integer.\n", signal.name);
            break;
        case ID_DATATYPE_DOUBLE:
            LOG_WARN("Decoding double signal '%s' as integer.\n", signal.name);
            break;
        case ID_DATATYPE_BOOL:
            LOG_WARN("Decoding boolean signal '%s' as integer.\n", signal.name);
            break;
        default:
            LOG_WARN("Decoding unknown signal '%s' as integer.\n", signal.name);
            break;
        }
        
        // Mask integer
        int64_t dataBuffer = (data >> signal.bitPosition) & signal.bitMask;
        
        // Sign extension
        if((dataBuffer >> (signal.bitLength - 1)) == 0b1)
        {
            dataBuffer |= ~signal.bitMask;
        }

        // Scale factor & offset
        dataBuffer = static_cast<int64_t>(dataBuffer * signal.scaleFactor + signal.offset);

        return dataBuffer;
    }

    double CanSocket::decodeDouble(const uint64_t& data, const CanSignal& signal)
    {
        switch(signal.datatypeId)
        {
        case ID_DATATYPE_DOUBLE:
            break;
        case ID_DATATYPE_UINT:
            LOG_WARN("Decoding unsigned integer signal '%s' as double.\n", signal.name);
            break;
        case ID_DATATYPE_INT:
            LOG_WARN("Decoding integer signal '%s' as double.\n", signal.name);
            break;
        case ID_DATATYPE_BOOL:
            LOG_WARN("Decoding boolean signal '%s' as double.\n", signal.name);
            break;
        default:
            LOG_WARN("Decoding unknown signal '%s' as double.\n", signal.name);
            break;
        }

        double dataBuffer = 0;

        if(signal.signedness)
        {
            // Mask integer
            int64_t intBuffer = (data >> signal.bitPosition) & signal.bitMask;
            
            // Sign extension
            if((intBuffer >> (signal.bitLength - 1)) == 0b1)
            {
                intBuffer |= ~signal.bitMask;
            }

            // Scale factor & offset
            dataBuffer = intBuffer * signal.scaleFactor + signal.offset;
        }
        else
        {
            // Mask integer
            uint64_t intBuffer = (data >> signal.bitPosition) & signal.bitMask;

            // Scale factor & offset
            dataBuffer = intBuffer * signal.scaleFactor + signal.offset;
        }

        return dataBuffer;
    }

    bool CanSocket::decodeBool(const uint64_t& data, const CanSignal& signal)
    {
        switch(signal.datatypeId)
        {
        case ID_DATATYPE_BOOL:
            break;
        case ID_DATATYPE_UINT:
            LOG_WARN("Decoding unsigned integer signal '%s' as boolean.\n", signal.name);
            break;
        case ID_DATATYPE_INT:
            LOG_WARN("Decoding integer signal '%s' as boolean.\n", signal.name);
            break;
        case ID_DATATYPE_DOUBLE:
            LOG_WARN("Decoding double signal '%s' as boolean.\n", signal.name);
            break;
        default:
            LOG_WARN("Decoding unknown signal '%s' as boolean.\n", signal.name);
            break;
        }
        
        // Mask bit
        uint64_t dataBuffer = (data >> signal.bitPosition) & signal.bitMask;

        return dataBuffer == 1;
    }

    uint64_t CanSocket::encodeUnsignedInt(const uint64_t& data, const CanSignal& signal)
    {
        switch(signal.datatypeId)
        {
        case ID_DATATYPE_UINT:
            break;
        case ID_DATATYPE_BOOL:
            LOG_WARN("Encoding boolean signal '%s' as unsigned int.\n", signal.name);
            break;
        case ID_DATATYPE_INT:
            LOG_WARN("Encoding integer signal '%s' as unsigned int.\n", signal.name);
            break;
        case ID_DATATYPE_DOUBLE:
            LOG_WARN("Encoding double signal '%s' as unsigned int.\n", signal.name);
            break;
        default:
            LOG_WARN("Encoding unknown signal '%s' as unsigned int.\n", signal.name);
            break;
        }
        
        uint64_t dataBuffer = static_cast<uint64_t>((data - signal.offset) / signal.scaleFactor);
        uint64_t bitBuffer = (dataBuffer & signal.bitMask) << signal.bitPosition;
        
        return bitBuffer;
    }

    uint64_t CanSocket::encodeSignedInt(const int64_t& data, const CanSignal& signal)
    {
        switch(signal.datatypeId)
        {
        case ID_DATATYPE_INT:
            break;
        case ID_DATATYPE_UINT:
            LOG_WARN("Encoding unsigned integer signal '%s' as integer.\n", signal.name);
            break;
        case ID_DATATYPE_BOOL:
            LOG_WARN("Encoding boolean signal '%s' as integer.\n", signal.name);
            break;
        case ID_DATATYPE_DOUBLE:
            LOG_WARN("Encoding double signal '%s' as integer.\n", signal.name);
            break;
        default:
            LOG_WARN("Encoding unknown signal '%s' as integer.\n", signal.name);
            break;
        }

        int64_t dataBuffer = static_cast<int64_t>((data - signal.offset) / signal.scaleFactor);
        uint64_t bitBuffer = (dataBuffer & signal.bitMask) << signal.bitPosition;
        
        return bitBuffer;
    }

    uint64_t CanSocket::encodeDouble(const double& data, const CanSignal& signal)
    {
        switch(signal.datatypeId)
        {
        case ID_DATATYPE_DOUBLE:
            break;
        case ID_DATATYPE_UINT:
            LOG_WARN("Encoding unsigned integer signal '%s' as double.\n", signal.name);
            break;
        case ID_DATATYPE_INT:
            LOG_WARN("Encoding integer signal '%s' as double.\n", signal.name);
            break;
        case ID_DATATYPE_BOOL:
            LOG_WARN("Encoding boolean signal '%s' as double.\n", signal.name);
            break;
        default:
            LOG_WARN("Encoding unknown signal '%s' as double.\n", signal.name);
            break;
        }

        double dataBuffer = (data - signal.offset) / signal.scaleFactor;
        uint64_t bitBuffer = 0;

        if(signal.signedness)
        {
            int64_t intBuffer = round(dataBuffer);
            bitBuffer = (intBuffer & signal.bitMask) << signal.bitPosition;
        }
        else
        {
            uint64_t intBuffer = round(dataBuffer);
            bitBuffer = (intBuffer & signal.bitMask) << signal.bitPosition;
        }

        return bitBuffer;
    }

    uint64_t CanSocket::encodeBool(const bool& data, const CanSignal& signal)
    {
        switch(signal.datatypeId)
        {
        case ID_DATATYPE_BOOL:
            break;
        case ID_DATATYPE_INT:
            LOG_WARN("Encoding integer signal '%s' as boolean.\n", signal.name);
            break;
        case ID_DATATYPE_UINT:
            LOG_WARN("Encoding unsigned integer signal '%s' as boolean.\n", signal.name);
            break;
        case ID_DATATYPE_DOUBLE:
            LOG_WARN("Encoding double signal '%s' as boolean.\n", signal.name);
            break;
        default:
            LOG_WARN("Encoding unknown signal '%s' as boolean.\n", signal.name);
            break;
        }

        uint64_t dataBuffer = (data & signal.bitMask) << signal.bitPosition;

        return dataBuffer;
    }

    void reallocateMessages(CanSignal** signalArray, CanMessage** messageArray, size_t oldSignalCount, size_t newSignalCount, size_t oldMessageCount, size_t newMessageCount)
    {
        LOG_INFO("Reallocating messages. Signal count: %lu => %lu, Message count: %lu => %lu...\n", oldSignalCount, newSignalCount, oldMessageCount, newMessageCount);

        // Validate parameters
        if(newSignalCount >= oldSignalCount || newMessageCount >= oldMessageCount)
        {
            LOG_ERROR("Failed to reallocate CAN message array: The new size must be less than the old size.\n");
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
            newSignalArray[index].datatypeId   = (*signalArray)[index].datatypeId;
            newSignalArray[index].bitPosition  = (*signalArray)[index].bitPosition;
            newSignalArray[index].bitLength    = (*signalArray)[index].bitLength;
            newSignalArray[index].bitMask      = (*signalArray)[index].bitMask;
            newSignalArray[index].scaleFactor  = (*signalArray)[index].scaleFactor;
            newSignalArray[index].offset       = (*signalArray)[index].offset;
            newSignalArray[index].signedness   = (*signalArray)[index].signedness;
        }

        // Delete old arrays
        delete [] *signalArray;
        delete [] *messageArray;

        // Update references
        *signalArray  = newSignalArray;
        *messageArray = newMessageArray;
    }
}
