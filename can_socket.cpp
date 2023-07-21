// Header
#include "can_socket.h"

// C Standard Libraries
#include "time.h"

CanSocket::CanSocket(std::string interfaceName)
{
    // Create the CAN socket
    this->socketDescriptor = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(this->socketDescriptor == -1)
    {
        this->errorCode = errno;
        throw std::runtime_error("Failed to create CAN socket for interface \"" + interfaceName + "\": " + std::string(strerror(this->errorCode)));
    }

    struct ifreq interface;
    
    // Set interface name, set terminating character
    strncpy(interface.ifr_name, interfaceName.c_str(), IFNAMSIZ - 1);
    interface.ifr_name[IFNAMSIZ - 1] = '\0';

    // Get the interface index from the name
    this->errorCode = ioctl(this->socketDescriptor, SIOCGIFINDEX, &interface);
    if(this->errorCode == -1)
    {
        this->errorCode = errno;
        throw std::runtime_error("Failed to get index of CAN interface \"" + interfaceName + "\": " + std::string(strerror(this->errorCode)));
    }

    // Allocate the CAN socket address
    struct sockaddr_can address =
    {
        AF_CAN,                 // CAN Address family
        interface.ifr_ifindex   // CAN Interface index
        // The can_addr field will be set later
    };

    // Bind the socket to the specified address
    this->errorCode = bind(this->socketDescriptor, reinterpret_cast<sockaddr*>(&address), static_cast<socklen_t>(sizeof(address)));
    if(this->errorCode == -1)
    {
        this->errorCode = errno;
        throw std::runtime_error("Failed bind CAN socket at interface \"" + interfaceName + "\" to index \"" + std::to_string(interface.ifr_ifindex) + "\": " + std::string(strerror(this->errorCode)));
    }
}

void CanSocket::send_m(unsigned int& id, uint64_t& data, unsigned int& dataCount)
{
    // Validate data count
    if(dataCount > 8) throw std::invalid_argument("Failed to send message: Data count cannot be greater than 8 bytes.");

    // Allocate message frame
    struct can_frame messageFrame =
    {
        id,                           // Message ID
        static_cast<__u8>(dataCount), // Byte length
    };

    // Copy message data
    for(int index = 0; index < dataCount; ++index)
    {
        messageFrame.data[index] = (data >> (index * 8)) & 0xFF;
    }

    // Send message frame
    this->errorCode = write(this->socketDescriptor, &messageFrame, sizeof(struct can_frame));
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

void CanSocket::read_m(unsigned int& id, uint64_t& data, unsigned int& dataCount)
{
    // Allocate message frame
    struct can_frame messageFrame;

    // Read message
    this->errorCode = read(this->socketDescriptor, &messageFrame, sizeof(struct can_frame));
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
    id = messageFrame.can_id;
    dataCount = static_cast<unsigned int>(messageFrame.len);

    // Validate data count
    if(dataCount > 8) throw std::runtime_error("Failed to read CAN message: Invalid data count.");

    // Copy message data
    data = 0x0;
    for(int index = 0; index < dataCount; ++index)
    {
        data |= (static_cast<uint64_t>(messageFrame.data[index]) << (index * 8));
    }
}

void CanSocket::setTimeout(unsigned long int timeMs)
{
    struct timeval timeout {0, 0};

    if(timeMs != 0)
    {
        timeout.tv_usec = (timeMs % 1000) * 1000;
        timeout.tv_sec = timeMs / 1000;
    }

    this->errorCode = setsockopt(this->socketDescriptor, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<void*>(&timeout), static_cast<socklen_t>(sizeof(timeout)));
    if(this->errorCode != 0)
    {
        this->errorCode = errno;
        throw std::runtime_error("Failed to set CAN Socket timeout: " + std::string(strerror(this->errorCode)));
    }
}