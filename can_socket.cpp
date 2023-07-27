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
        // Create the CAN socket
        this->descriptor = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if(this->descriptor == -1)
        {
            this->errorCode = errno;
            throw std::runtime_error(std::string("Failed to create CAN socket for interface \"") + deviceName + "\": " + strerror(this->errorCode));
        }

        struct ifreq interface;
        
        // Set interface name, set terminating character if name exceeds bounds
        strncpy(interface.ifr_name, deviceName, IFNAMSIZ - 1);
        interface.ifr_name[IFNAMSIZ - 1] = '\0';

        // Get the interface index from the name
        this->errorCode = ioctl(this->descriptor, SIOCGIFINDEX, &interface);
        if(this->errorCode == -1)
        {
            this->errorCode = errno;
            throw std::runtime_error(std::string("Failed to get index of CAN interface \"") + deviceName + "\": " + std::string(strerror(this->errorCode)));
        }

        // Allocate the CAN socket address
        struct sockaddr_can address =
        {
            AF_CAN,                 // CAN Address family
            interface.ifr_ifindex   // CAN Interface index
            // The can_addr field will be set later
        };

        // Bind the socket to the specified address
        this->errorCode = bind(this->descriptor, reinterpret_cast<sockaddr*>(&address), static_cast<socklen_t>(sizeof(address)));
        if(this->errorCode == -1)
        {
            this->errorCode = errno;
            throw std::runtime_error(std::string("Failed bind CAN socket at interface \"") + deviceName + "\" to index \"" + std::to_string(interface.ifr_ifindex) + "\": " + std::string(strerror(this->errorCode)));
        }
    }

    void CanSocket::sendMessage(const unsigned char* data, const size_t* dataLength, const unsigned int* id)
    {
        // Validate data count
        if(*dataLength > 8) throw std::invalid_argument("Failed to send message: Data count cannot be greater than 8 bytes.");

        // Allocate message frame
        struct can_frame messageFrame =
        {
            *id,                            // Message ID
            static_cast<__u8>(*dataLength), // Byte length
        };

        // Copy message data
        for(int index = 0; index < *dataLength; ++index)
        {
            messageFrame.data[index] = data[index];
        }

        // Send message frame
        this->errorCode = write(this->descriptor, &messageFrame, sizeof(struct can_frame));
        if(this->errorCode < sizeof(struct can_frame))
        {
            this->errorCode = errno;

            // Check for timeout
            if(this->errorCode == EAGAIN || this->errorCode == EWOULDBLOCK || this->errorCode == EINPROGRESS)
            {
                throw Timeout();
            }
            
            throw std::runtime_error("Failed send CAN message: Sent incomplete message. " + std::string(strerror(this->errorCode)));
        }
    }

    void CanSocket::readMessage(unsigned char* data, size_t* dataLength, unsigned int* id)
    {
        // Allocate message frame
        struct can_frame messageFrame;

        // Read message
        this->errorCode = read(this->descriptor, &messageFrame, sizeof(struct can_frame));
        if(this->errorCode < 0) {
            this->errorCode = errno;

            // Check for timeout
            if(this->errorCode == EAGAIN || this->errorCode == EWOULDBLOCK || this->errorCode == EINPROGRESS)
            {
                throw Timeout();
            }

            throw std::runtime_error("Failed read CAN message: " + std::string(strerror(this->errorCode)));
        }

        // Check message validity
        if(this->errorCode < sizeof(struct can_frame)) {
            this->errorCode = errno;
            throw std::runtime_error("Failed read CAN message: Received incomplete message. " + std::string(strerror(this->errorCode)));
        }

        // Read metadata
        *id = messageFrame.can_id;
        *dataLength = static_cast<unsigned int>(messageFrame.len);

        // Validate data count
        if(*dataLength > 8) throw std::runtime_error("Failed to read CAN message: Invalid data count.");

        // Copy message data
        for(int index = 0; index < *dataLength; ++index)
        {
            data[index] = messageFrame.data[index];
        }
    }

    template<typename T>
    T CanSocket::parseSignal(uint64_t* data, CanSignal* signal)
    {
        *data = (*data >> signal->bitPosition) & signal->bitMask;
        return *reinterpret_cast<T*>(data);
    }

    // template<>
    // float CanSocket::parseSignal(uint64_t* data, CanSignal* signal)
    // {
    //     *data = (*data >> signal->bitPosition) & signal->bitMask;
    //     return (data);
    // }

    // TODO: This needs a better place to go?
    void CanSocket::reallocateMessages(CanSignal** signalArray, CanMessage** messageArray, size_t oldSignalCount, size_t newSignalCount, size_t oldMessageCount, size_t newMessageCount)
    {
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
    }

    // Template Forward Declarations ---------------------------------------------------------------------------------------------- 

    template int                    CanSocket::parseSignal(uint64_t* data, CanSignal* signal);

    template unsigned int           CanSocket::parseSignal(uint64_t* data, CanSignal* signal);

    template long int               CanSocket::parseSignal(uint64_t* data, CanSignal* signal);

    template long unsigned int      CanSocket::parseSignal(uint64_t* data, CanSignal* signal);

    template long long int          CanSocket::parseSignal(uint64_t* data, CanSignal* signal);

    template long long unsigned int CanSocket::parseSignal(uint64_t* data, CanSignal* signal);

    template bool                   CanSocket::parseSignal(uint64_t* data, CanSignal* signal);

    template char                   CanSocket::parseSignal(uint64_t* data, CanSignal* signal);

    template unsigned char          CanSocket::parseSignal(uint64_t* data, CanSignal* signal);

    template float                  CanSocket::parseSignal(uint64_t* data, CanSignal* signal);

    template double                 CanSocket::parseSignal(uint64_t* data, CanSignal* signal);
}
