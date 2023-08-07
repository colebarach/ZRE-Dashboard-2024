#ifndef CAN_SOCKET_H
#define CAN_SOCKET_H

// CAN Socket -----------------------------------------------------------------------------------------------------------------
//
// Author: Cole Barach
//
// Description: An interface for CAN devices based on the Linux SocketCAN implementation.
//
// Created: 23.07.08
// Updated: 23.07.30
//
// To do:
// - The parse function needs to account for offset and scale factor. This is kinda weird, how do bools and chars get treated?
// - Consider a static size for message and signal names.
// - Implement signal endianness, not sure how this works in CAN yet
// - The reallocate message function should be moved, not sure where yet
// - Encode and decode make more sense than parse, minor issue though
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

// C++ Standard Libraries
#include <vector>
#include <string>

// Network Namespace ----------------------------------------------------------------------------------------------------------

namespace Network
{
    // Compilation Flags ------------------------------------------------------------------------------------------------------

    #define DEBUG_GENERAL
    // #define DEBUG_TRAFFIC
    // #define DEBUG_PARSE
    #define DEBUG_DATATYPE_WARNING

    // Constants --------------------------------------------------------------------------------------------------------------

    #define ID_DATATYPE_UINT   0
    #define ID_DATATYPE_INT    1
    #define ID_DATATYPE_BOOL   2
    #define ID_DATATYPE_DOUBLE 3

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
        // - Call to broadcast message to the bound CAN bus
        // - Will send the message using the provided paramaters
        void sendMessage(const uint64_t* data, const uint8_t* dataLength, const uint16_t* id);

        // Read CAN Message
        // - Call to read the next message from the bound CAN bus
        // - Will write the message parameters to the provided variables
        void readMessage(uint64_t* data, uint8_t* dataLength, uint16_t* id);

        // Parse Signal
        // - Call to get the value of a signal from a CAN message's data
        // - Returns the parsed datatype
        static uint64_t parseUnsignedInt(const uint64_t& data, const CanSignal& signal);
        static int64_t  parseSignedInt(const uint64_t& data, const CanSignal& signal);
        static double   parseDouble(const uint64_t& data, const CanSignal& signal);
        static bool     parseBool(const uint64_t& data, const CanSignal& signal);

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
        // - Can only shrink a set of messages, will throw a standard exception if an attempt is made to grow a set
        static void reallocateMessages(CanSignal** signalArray, CanMessage** messageArray, size_t oldSignalCount, size_t newSignalCount, size_t oldMessageCount, size_t newMessageCount);
    };
}

#endif // CAN_SOCKET_H