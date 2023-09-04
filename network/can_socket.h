#ifndef CAN_SOCKET_H
#define CAN_SOCKET_H

// CAN Socket -----------------------------------------------------------------------------------------------------------------
//
// Author: Cole Barach
//
// Description: An interface for CAN devices based on the Linux SocketCAN implementation.
//
// Created: 23.07.08
// Updated: 23.08.28
//
// To do:
// - The encode function needs to account for offset and scale factor. This is kinda weird, how do bools and chars get treated?
// - Consider a static size for message and signal names.
// - Implement signal endianness, not sure how this works in CAN yet
// - The reallocate message function should be moved, not sure where yet
//
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

// C Standard Libraries
#include <stdint.h>

// Network Namespace ----------------------------------------------------------------------------------------------------------

namespace Network
{
    // Constants --------------------------------------------------------------------------------------------------------------

    #define ID_DATATYPE_UINT   0         // Unsigned integer
    #define ID_DATATYPE_INT    1         // Signed integer
    #define ID_DATATYPE_BOOL   2         // Boolean
    #define ID_DATATYPE_DOUBLE 3         // Double-precision float

    // Structures -------------------------------------------------------------------------------------------------------------

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
        uint16_t     id;                     // Message ID
    };

    // CAN Message
    // - A single variable contained in a message
    struct CanSignal
    {
        CanMessage*  messageArray;           // Array of CAN messages
        size_t       messageIndex;           // Index of the parent CAN message
        char*        name;                   // Name of the signal, c-string
        int          datatypeId;             // ID of the signal datatype
        uint16_t     bitPosition;            // Bit position, position of first bit in message
        uint16_t     bitLength;              // Bit length, number of bits data occupies
        uint64_t     bitMask;                // Bitmask, (2 ^ bitLength - 1)
        double       scaleFactor;            // 
        double       offset;                 // 
        bool         signedness;             // 0 => unsigned, 1 => signed
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
        // - Call to broadcast message to the bound CAN bus
        // - Will send the message using the provided paramaters
        void sendMessage(const uint64_t* data, const uint8_t* dataLength, const uint16_t* id);

        // Read CAN Message
        // - Call to read the next message from the bound CAN bus
        // - Will write the message parameters to the provided variables
        void readMessage(uint64_t* data, uint8_t* dataLength, uint16_t* id);

        // Decode Signal
        // - Call to get the value of a signal from a CAN message's data
        // - Returns the decoded datatype
        static uint64_t decodeUnsignedInt(const uint64_t& data, const CanSignal& signal);
        static int64_t  decodeSignedInt(const uint64_t& data, const CanSignal& signal);
        static double   decodeDouble(const uint64_t& data, const CanSignal& signal);
        static bool     decodeBool(const uint64_t& data, const CanSignal& signal);

        // Encode Signal
        // - Call to get the message data encoding the given signal value
        // - Returns the bitfield of the given signal, (all other bits are 0)
        static uint64_t encodeUnsignedInt(const uint64_t& data, const CanSignal& signal);
        static uint64_t encodeSignedInt(const int64_t& data, const CanSignal& signal);
        static uint64_t encodeDouble(const double& data, const CanSignal& signal);
        static uint64_t encodeBool(const bool& data, const CanSignal& signal);

        // Reallocate Messages
        // - Call to reallocate the signal and message arrays, maintaining internal references
        // - Accepts a pointer to a CAN signal array and a pointer to a CAN message array, the array's of which are replaced
        // - Currently can only shrink a set of messages, will throw a standard exception if an attempt is made to grow a set
        // - Can only be used on two dynamically allocated arrays
        static void reallocateMessages(CanSignal** signalArray, CanMessage** messageArray, size_t oldSignalCount, size_t newSignalCount, size_t oldMessageCount, size_t newMessageCount);
    };
}

#endif // CAN_SOCKET_H