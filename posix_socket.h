#ifndef POSIX_SOCKET_H
#define POSIX_SOCKET_H

// POSIX Socket ---------------------------------------------------------------------------------------------------------------

// Description: A wrapper class for the POSIX socket network interface. This object acts as a base class from which
//   implementation may be derived.

// Libraries ------------------------------------------------------------------------------------------------------------------

// C++ Libraries
#include <exception>
#include <stdexcept>

// C Standard Libraries
#include <stddef.h>

namespace Network
{
    class PosixSocket
    {
        // Constructor / Destructor -----------------------------------------------------------------------------------------------

        public:

        PosixSocket();

        // Public Functions -------------------------------------------------------------------------------------------------------

        // Set Timeout Milliseconds
        // - Set the send and read function timeouts in milliseconds
        // - Use 0 ms to disable a set timeout
        void setTimeoutMs(unsigned long int timeMs);

        // Exceptions -------------------------------------------------------------------------------------------------------------
        
        class ConnectionEnd : public std::exception
        {
            const char* what() const noexcept
            {
                return "The remote socket has ended the connection.";
            }
        };

        class Timeout : public std::exception
        {
            const char* what() const noexcept
            {
                return "A timeout occured.";
            }
        };

        // Protected Variables ----------------------------------------------------------------------------------------------------

        protected:

        int descriptor;                      // Socket file descriptor
        int errorCode;                       // Generic error code, used to store errno in exceptions

        // Protected Functions ----------------------------------------------------------------------------------------------------
    };
}

#endif // POSIX_SOCKET_H