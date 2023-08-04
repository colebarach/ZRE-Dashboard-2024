// Header
#include "can_dbc.h"

// C++ Standard Libraries
#include <fstream>
#include <iostream>
#include <iomanip>

// C Standard Libraries
#include <string.h>

namespace Network
{
    void CanDbc::parseFile(std::string filePath, CanMessage** messages, CanSignal** signals, size_t* messageCount, size_t* signalCount)
    {
        #ifdef DEBUG_GENERAL
        std::cout << "Loading dbc file \"" << filePath << "\"..." << std::endl;
        #endif
        
        // Open file for reading
        std::ifstream file(filePath, std::ios_base::in);
        std::string data;

        // Allocate message array
        *messages = new CanMessage[MAX_SIZE_MESSAGE_ARRAY];
        *messageCount = 0;

        // Allocate signal array
        *signals = new CanSignal[MAX_SIZE_SIGNAL_ARRAY];
        *signalCount = 0;

        CanMessage* message = nullptr;

        while(true)
        {
            file >> data;

            if(file.eof()) break;

            if(data == DBC_KEYWORD_ECU)
            {
                // Ignore remainder of line
                std::getline(file, data);
            }
            else if(data == DBC_KEYWORD_MESSAGE)
            {
                if(*messageCount == MAX_SIZE_MESSAGE_ARRAY)
                {
                    throw std::runtime_error("Failed to parse DBC file: The file exceeds the maximum number of messages (" + std::to_string(MAX_SIZE_MESSAGE_ARRAY) + ").");
                }

                message = &(*messages)[*messageCount];

                ++(*messageCount);

                message->signalArray = *signals;
                message->signalIndex = *signalCount;
                message->signalCount = 0;

                // Read message ID 
                file >> message->id;

                // Read message name
                file >> data;
                if(data.size() == 0)
                {
                    throw std::runtime_error("Failed to parse DBC file: Read empty message name.");
                }
                message->name = new char[data.size() + 1];
                strcpy(message->name, data.c_str());

                // Ignore DLC
                file >> data;

                // Ignore message sending ECU
                file >> data;
            }
            else if(data == DBC_KEYWORD_SIGNAL)
            {
                if(message == nullptr)
                {
                    throw std::runtime_error("Failed to parse DBC file: Read a signal before reading the first message.");
                }

                if(*signalCount == MAX_SIZE_SIGNAL_ARRAY)
                {
                    throw std::runtime_error("Failed to parse DBC file: The file exceeds the maximum number of signals (" + std::to_string(MAX_SIZE_SIGNAL_ARRAY) + ").");
                }

                // Add signal
                CanSignal* signal = &(*signals)[*signalCount];
                ++(*signalCount);
                ++message->signalCount;

                signal->messageArray = (*messages);
                signal->messageIndex = *messageCount - 1;
                
                // Format: SG_ <Name> : <Start bit position>|<Bit length>@<sign> (<Scale factor>,<Offset>) [<Min>|<Max>] "<Unit>" <ECU>
                std::getline(file, data);
                
                // Partition string
                size_t nameStart   = 1;                            // Keyword was already read, line now starts with name, +1 for leading space
                size_t nameEnd     = data.find(':') - 1;           // Name ends at the colon, -1 for space
                size_t bitPosStart = nameEnd + 3;                  // Bit pos starts after name, +3 for " : "
                size_t bitPosEnd   = data.find('|');               // Bit pos ends at '|'
                size_t bitLenStart = bitPosEnd + 1;                // Bit len starts after '|'
                size_t bitLenEnd   = data.find('@');               // Bit len ends at '@'
                size_t signStart   = bitLenEnd + 1;                // Sign starts after bit len
                size_t signEnd     = data.find('(') - 1;           // Sign ends before '(', +1 for space
                // size_t scaleStart  = signEnd + 2;                  // Scale starts after '('
                // size_t scaleEnd    = data.find(',');               // Scale ends at ','
                // size_t offsetStart = scaleEnd + 1;                 // Offset starts after ','
                // size_t offsetEnd   = data.find(')');               // Offset ends at ')'
                // size_t minStart    = offsetEnd + 3;                // Min starts 3 characters after offset
                // size_t minEnd      = data.find('|', minStart);     // Min ends with '|', second instance of this in data
                // size_t maxStart    = minEnd + 1;                   // Max starts after '|', +1 char
                // size_t maxEnd      = data.find(']');               // Max ends with ']'
                // size_t unitStart   = data.find('"')+1;             // Unit starts with '""
                // size_t unitEnd     = data.find('"', unitStart);    // Unit ends with second instance of '"'
                // size_t ecuStart    = unitEnd + 3;                  // ECU starts after unit, +3 for '"' and 2 spaces
                // size_t ecuEnd      = data.length()-1;              // ECU ends with line

                #ifdef DEBUG_ENTRY_INTERPRET
                std::cout << "SIGNAL PARTITION: "
                << " NAME = '"    << data.substr(nameStart,   nameEnd   - nameStart)   << "'"
                << " BIT POS = '" << data.substr(bitPosStart, bitPosEnd - bitPosStart) << "'"
                << " BIT LEN = '" << data.substr(bitLenStart, bitLenEnd - bitLenStart) << "'"
                << " SIGN = '"    << data.substr(signStart,   signEnd   - signStart)   << "'"
                << " SCALE = '"   << data.substr(scaleStart,  scaleEnd  - scaleStart)  << "'"
                << " OFFSET = '"  << data.substr(offsetStart, offsetEnd - offsetStart) << "'"
                << " MIN = '"     << data.substr(minStart,    minEnd    - minStart)    << "'"
                << " MAX = '"     << data.substr(maxStart,    maxEnd    - maxStart)    << "'"
                << " UNIT = '"    << data.substr(unitStart,   unitEnd   - unitStart)   << "'"
                << " ECU = '"     << data.substr(ecuStart,    ecuEnd    - ecuStart)    << "'" << std::endl;
                #endif

                // Read name
                std::string signalName = data.substr(nameStart, nameEnd - nameStart);
                signal->name = new char[signalName.size() + 1];
                if(signalName.size() == 0)
                {
                    throw std::runtime_error("Failed to parse DBC file: Read empty signal name.");
                }
                strcpy(signal->name, signalName.c_str());

                // Read bit position
                signal->bitPosition = atoi(data.substr(bitPosStart, bitPosEnd - bitPosStart).c_str());
                
                // Read bit length
                signal->bitLength = atoi(data.substr(bitLenStart, bitLenEnd - bitLenStart).c_str());
                
                // Populate bitmask
                signal->bitMask = 0U;
                for(size_t index = 0; index < signal->bitLength; ++index)
                {
                    signal->bitMask |= 1U << index;
                }

                // Read sign ("1+" => unsigned, "1-" => signed)
                signal->isSigned = (data.substr(signStart, signEnd - signStart) == "1-");

                // TODO: FLOATS
                if(signal->bitLength == 1)
                {
                    signal->datatypeId = ID_DATATYPE_BOOL;
                }
                else if(signal->isSigned)
                {
                    signal->datatypeId = ID_DATATYPE_INT;
                }
                else
                {
                    signal->datatypeId = ID_DATATYPE_UINT;
                }
            }
            else if(data == DBC_KEYWORD_COMMENT)
            {
                // Ignore remainder of line.
                std::getline(file, data);
            }
            else if(data == DBC_KEYWORD_VAL_TABLE)
            {
                // Ignore remainder of line
                std::getline(file, data);
            }
            else if(data == DBC_KEYWORD_VERSION)
            {
                // Ignore remainder of line
                std::getline(file, data);
            }
            else if(data == DBC_KEYWORD_MISC)
            {
                // Ignore all following lines starting with tab
                char nextChar = ' ';
                while(nextChar == ' ' || nextChar == '\t')
                {
                    std::getline(file, data);

                    nextChar = file.peek();
                }
            }
            else if(data == DBC_KEYWORD_MISC2)
            {
                // Ignore
            }
            else if(data == DBC_KEYWORD_MISC3 || data == DBC_KEYWORD_MISC4 || data == DBC_KEYWORD_MISC5)
            {
                // Ignore line
                std::getline(file, data);
            }
            else
            {
                #ifdef DEBUG_GENERAL
                std::cout << "Unknown keyword \"" << data << "\". Ignoring..." << std::endl;
                #endif
            }
        }

        #ifdef DEBUG_GENERAL
        std::cout << "Loaded." << std::endl;
        #endif
    }
}