#ifndef CAN_DBC_H
#define CAN_DBC_H

// CAN DBC --------------------------------------------------------------------------------------------------------------------
//
// Author: Cole Barach
//
// Description: A group of functions relating to CAN DBC files.
//
// Created: 23.07.19
// Updated: 23.09.04
//
// References:
// - http://mcu.so/Microcontroller/Automotive/dbc-file-format-documentation_compress.pdf
// - https://www.csselectronics.com/pages/can-dbc-file-database-intro
// - https://docs.fileformat.com/database/dbc/

// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "can_socket.h"

namespace Network
{
    namespace CanDbc
    {
        // Compilation Flags --------------------------------------------------------------------------------------------------

        #define LOG_ENTRY_PARSING     // Print the interpretation of the database to the standard output. Use for debugging.

        // Constants ----------------------------------------------------------------------------------------------------------

        #define MAX_SIZE_SIGNAL_ARRAY  4096
        #define MAX_SIZE_MESSAGE_ARRAY 1024
        #define SIZE_DATA_BUFFER       4096
        
        #define STRINGIZE(x)           #x
        #define SIZE_DATA_BUFFER_STR   STRINGIZE(SIZE_DATA_BUFFER)

        // DBC Keywords -------------------------------------------------------------------------------------------------------

        #define DBC_KEYWORD_NETWORK_NODE "BU_:"          // CAN ECU
        #define DBC_KEYWORD_MESSAGE      "BO_"           // CAN message
        #define DBC_KEYWORD_SIGNAL       "SG_"           // CAN signal
        #define DBC_KEYWORD_ENV_VARIABLE "EV_"           // Environment variable
        #define DBC_KEYWORD_SIG_GROUP    "SIG_GROUP_"    // Signal group
        #define DBC_KEYWORD_VAL_TABLE    "VAL_TABLE_"    // Value table
        #define DBC_KEYWORD_VERSION      "VERSION"       // Version Number
        #define DBC_KEYWORD_BIT_TIMING   "BS_:"          // Network baudrate, obsolete
        #define DBC_KEYWORD_COMMENT      "CM_"           // Comments, ignored for now

        #define DBC_KEYWORD_NS           "NS_"           // Purpose unknown, ignored for now

        // Parse File
        // - Call to parse the contents of a DBC file into CAN Messages and Signals
        // - Allocates dynamic arrays storing the new messages and signals
        // - Updates the message and signal pointers to refer to the new arrays
        void parseFile(const char* filePath, CanMessage** messages, CanSignal** signals, size_t* messageCount, size_t* signalCount);
    }
}

#endif // CAN_DBC_H