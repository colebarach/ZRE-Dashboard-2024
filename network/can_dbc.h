#ifndef CAN_DBC_H
#define CAN_DBC_H

// CAN DBC --------------------------------------------------------------------------------------------------------------------
//
// Author: Cole Barach
//
// Description: A group of functions relating to CAN DBC files.
//
// Created: 23.07.19
// Updated: 23.07.27
//
// To do:
// - Scale factor and offset are ignored, they need to be used eventually
// - Document DBC Keywords and line formats
// - Message and signal arrays need reallocated when this is done parsing, otherwise memory is being wasted.
//
// References:
// - http://mcu.so/Microcontroller/Automotive/dbc-file-format-documentation_compress.pdf

// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "can_socket.h"

// C++ Standard Libraries
#include <string>
#include <vector>

namespace Network
{
    namespace CanDbc
    {
        // Compilation Flags --------------------------------------------------------------------------------------------------

        // #define DEBUG_GENERAL
        // #define DEBUG_ENTRY_INTERPRET

        // Constants ----------------------------------------------------------------------------------------------------------

        #define MAX_SIZE_SIGNAL_ARRAY  4096
        #define MAX_SIZE_MESSAGE_ARRAY 1024

        // DBC Keywords -------------------------------------------------------------------------------------------------------

        #define DBC_KEYWORD_ECU       "BU_:"         // Network Node
        #define DBC_KEYWORD_MESSAGE   "BO_"          // Message object
        #define DBC_KEYWORD_SIGNAL    "SG_"          // Signal object
        #define DBC_KEYWORD_VARIABLE  "EV_"          // Environment variable
        #define DBC_KEYWORD_SIG_GROUP "SIG_GROUP_"   // Signal group
        #define DBC_KEYWORD_VAL_TABLE "VAL_TABLE_"   // Value table
        #define DBC_KEYWORD_VERSION   "VERSION"      // File version
        #define DBC_KEYWORD_COMMENT   "CM_"          // Object comment
        #define DBC_KEYWORD_MISC      "NS_"          // TODO: Not sure
        #define DBC_KEYWORD_MISC2     "BS_:"         // TODO: Not sure
        #define DBC_KEYWORD_MISC3     "BA_DEF_"      // TODO: Not sure
        #define DBC_KEYWORD_MISC4     "BA_DEF_DEF_"  // TODO: Not sure
        #define DBC_KEYWORD_MISC5     "VAL_"         // TODO: Not sure

        // Parse File
        // - Call to parse the contents of a DBC file into CAN Messages and Signals
        // - Allocates dynamic arrays storing the new messages and signals
        // - Updates the message and signal pointers to refer to the new arrays
        void parseFile(const char* filePath, CanMessage** messages, CanSignal** signals, size_t* messageCount, size_t* signalCount);
    }
}

#endif // CAN_DBC_H