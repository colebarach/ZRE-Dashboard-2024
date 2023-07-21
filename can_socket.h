#ifndef CAN_SOCKET_H
#define CAN_SOCKET_H

// CAN Socket -----------------------------------------------------------------------------------------------------------------

// Description: An interface for CAN devices. Based on the Linux SocketCAN implementation.

// References:
// - https://www.kernel.org/doc/html/latest/networking/can.html
// - https://en.wikipedia.org/wiki/SocketCAN
// - https://www.man7.org/linux/man-pages/man2/socket.2.html
// - https://www.man7.org/linux/man-pages/man7/netdevice.7.html
// - https://www.gnu.org/software/libc/manual/html_node/Interface-Naming.html
// - https://www.can-cia.org/fileadmin/resources/documents/proceedings/2012_hartkopp.pdf
// - https://github.com/linux-can/can-utils/tree/master

// Libraries ------------------------------------------------------------------------------------------------------------------

// C++ Libraries
#include <string>
#include <iostream>
#include <exception>
#include <stdexcept>

// C Libraries
#include <string.h>

// POSIX Libraries
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

// SocketCAN Libraries
#include <linux/can.h>
#include <linux/can/raw.h>

// Classes --------------------------------------------------------------------------------------------------------------------

class CanSocket
{
    // Member Variables -------------------------------------------------------------------------------------------------------

    private:

    int socketDescriptor;                // CAN Socket file descriptor
    int errorCode;                       // Generic error code, used to store errno in exceptions

    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    public:

    // Constructor
    // - Creates a CAN socket bound to the given device
    // - Creates the socket, throws a runtime_error otherwise 
    CanSocket(std::string deviceName);

    // Member Functions -------------------------------------------------------------------------------------------------------

    // Send
    // - If a timeout is set, a Timeout exception may be thrown
    void send_m(unsigned int& id, uint64_t& data, unsigned int& dataCount);

    // Read
    // - If a timeout is set, a Timeout exception may be thrown
    void read_m(unsigned int& id, uint64_t& data, unsigned int& dataCount);

    // Set Timeout
    // - Set the send and read function timeouts in milliseconds
    // - Use 0 ms to disable a set timeout
    void setTimeout(unsigned long int timeMs);

    // Exceptions -------------------------------------------------------------------------------------------------------------

    class Timeout : public std::exception
    {
        const char* what() const noexcept
        {
            return "A timeout occured.";
        }
    };
};

#endif // CAN_SOCKET_H