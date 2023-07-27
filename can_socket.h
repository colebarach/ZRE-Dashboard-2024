#ifndef CAN_SOCKET_H
#define CAN_SOCKET_H

// CAN Socket -----------------------------------------------------------------------------------------------------------------

// Description: An interface for CAN devices based on the Linux SocketCAN implementation.

// To do:
// - The parse function needs to account for offset and scale factor. This is kinda weird, how do bools and chars get treated?
// - Type ID will need reimplemented for parsing. Trying to allocate memory before is weird though. Not sure how to approach
//   this yet.
// - Consider a static size for message and signal names.

// References:
// - https://www.kernel.org/doc/html/latest/networking/can.html
// - https://en.wikipedia.org/wiki/SocketCAN
// - https://www.man7.org/linux/man-pages/man2/socket.2.html
// - https://www.man7.org/linux/man-pages/man7/netdevice.7.html
// - https://www.gnu.org/software/libc/manual/html_node/Interface-Naming.html
// - https://www.can-cia.org/fileadmin/resources/documents/proceedings/2012_hartkopp.pdf
// - https://github.com/linux-can/can-utils/tree/master

// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "posix_socket.h"

// C++ Standard Libraries
#include <vector>
#include <string>

// C Standard Libraries
#include <cstdint>

// Structures -----------------------------------------------------------------------------------------------------------------

namespace Network
{
    // CAN Signal forward declaration
    struct CanSignal;

    // CAN Message
    // - A group of signals broadcast on a CAN bus
    struct CanMessage
    {
        CanSignal*   signalArray;            // Array of CAN signals
        size_t       signalIndex;            // Starting index of this message's CAN signals
        size_t       signalCount;            // Number of signals in this message
        char*        name;                   // Name of the message
        unsigned int id;                     // Message ID
    };

    // CAN Message
    // - A single variable contained in a message
    struct CanSignal
    {
        CanMessage*  messageArray;           // Array of CAN messages
        size_t       messageIndex;           // Index of the parent CAN message
        char*        name;                   // Name of the signal, c-string
        uint16_t     bitPosition;            // Bit position, position of first bit in message
        uint16_t     bitLength;              // Bit length, number of bits data occupies
        uint64_t     bitMask;                // Bitmask, (2 ^ bitLength - 1)
        bool         isSigned;               // Is signed, indicates whether data is signed before scale and offset
    };

    // Classes ----------------------------------------------------------------------------------------------------------------

    class CanSocket : public PosixSocket
    {
        // Constructor / Destructor -------------------------------------------------------------------------------------------

        public:

        // Constructor
        // - Creates a CAN socket bound to the given device
        // - Creates the socket, throws a runtime_error otherwise 
        CanSocket(const char* deviceName);

        // Public Functions ---------------------------------------------------------------------------------------------------

        // Send CAN Message
        // - Call to send a CAN message
        void sendMessage(const unsigned char* data, const size_t* dataLength, const unsigned int* id);

        // Read CAN Message
        // - Call to read a CAN message
        void readMessage(unsigned char* data, size_t* dataLength, unsigned int* id);

        // Parse CAN Signal
        // - Get the value of a CAN signal from its message data
        // - Returns the value in an 8 byte frame
        template<typename T>
        static T parseSignal(uint64_t* data, CanSignal* signal);

        // Reallocate Messages
        // - Call to reallocate the signal and message arrays, maintaining internal references
        // - Accepts a pointer to a CAN signal array and a pointer to a CAN message array, the array's of which are replaced
        // - Can only shrink a set of messages, will throw a standard exception if an attempt is made to grow a set
        static void reallocateMessages(CanSignal** signalArray, CanMessage** messageArray, size_t oldSignalCount, size_t newSignalCount, size_t oldMessageCount, size_t newMessageCount);
    };
}

#endif // CAN_SOCKET_H