#ifndef POSIX_SOCKET_H
#define POSIX_SOCKET_H

// POSIX Socket ---------------------------------------------------------------------------------------------------------------
//
// Author: Cole Barach
// 
// Description: A wrapper class for the POSIX socket network interface. This object acts as a base class from which
//   implementation may be derived.
// 
// Created: 23.06.??
// Updated: 23.07.30
//
// To do:
// - This is a pretty bare-bones implementation, there is a TCP socket class that has a lot of functionality that could be
//   moved into here, but this has to wait.
// - Documentation is sparse. Needs more love.

// Libraries ------------------------------------------------------------------------------------------------------------------

// C Standard Libraries
#include <stddef.h>

// C++ Libraries
#include <exception>
#include <stdexcept>

// Network Namespace ----------------------------------------------------------------------------------------------------------

namespace Network
{
    // Classes ----------------------------------------------------------------------------------------------------------------

    class PosixSocket
    {
        // Constructor / Destructor -------------------------------------------------------------------------------------------

        public:

        PosixSocket();

        // Public Functions ---------------------------------------------------------------------------------------------------

        // Set Timeout Milliseconds
        // - Set the send and read function timeouts in milliseconds
        // - Use 0 ms to disable a set timeout
        void setTimeoutMs(unsigned long int timeMs);

        // Exceptions ---------------------------------------------------------------------------------------------------------
        
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

        // Protected Variables ------------------------------------------------------------------------------------------------

        protected:

        int descriptor;                      // Socket file descriptor
        int errorCode;                       // Generic error code, used to store errno in exceptions

        // Protected Functions ------------------------------------------------------------------------------------------------
    };
}

#endif // POSIX_SOCKET_H