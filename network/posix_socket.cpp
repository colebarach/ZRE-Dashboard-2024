// Header
#include "posix_socket.h"

// Includes
#include "log.h"

// C++ Libraries
#include <string>

// C Standard Libraries
#include <errno.h>
#include <time.h>
#include <string.h>

// POSIX Libraries
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

namespace Network
{
    PosixSocket::PosixSocket() {}

    void PosixSocket::setTimeoutMs(unsigned long int timeMs)
    {
        if(this->descriptor == -1)
        {
            LOG_ERROR("Failed to set socket timeout: The socket descriptor is invalid.\n");
            throw std::runtime_error("Failed to set socket timeout: The socket descriptor is invalid.");
        }

        struct timeval timeout {0, 0};

        if(timeMs != 0)
        {
            timeout.tv_usec = (timeMs % 1000) * 1000;
            timeout.tv_sec = timeMs / 1000;
        }

        this->errorCode = setsockopt(this->descriptor, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<void*>(&timeout), static_cast<socklen_t>(sizeof(timeout)));
        if(this->errorCode != 0)
        {
            this->errorCode = errno;
            LOG_ERROR("Failed to set socket timeout: %s\n", strerror(this->errorCode));
            throw std::runtime_error("Failed to set socket timeout: " + std::string(strerror(this->errorCode)));
        }
    }
}
