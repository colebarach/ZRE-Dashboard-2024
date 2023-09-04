// Header
#include "can_dbc.h"

// Includes
#include "log.h"

// C Standard Libraries
#include <stdio.h>
#include <string.h>
#include <math.h>

namespace Network
{
    char dataBuffer[SIZE_DATA_BUFFER];
    char dataBuffer2[SIZE_DATA_BUFFER];
    char dataBuffer3[SIZE_DATA_BUFFER];
    int errorCode = -1;

    void CanDbc::parseFile(const char* filePath, CanMessage** messages, CanSignal** signals, size_t* messageCount, size_t* signalCount)
    {
        LOG_INFO("Loading DBC file '%s'...\n", filePath);

        // Open file for reading
        FILE* file = fopen(filePath, "r");
        if(file == NULL)
        {
            errorCode = errno;
            LOG_ERROR("Failed to parse DBC file: %s\n", strerror(errorCode));
            throw std::runtime_error("Failed to parse DBC file: " + std::string(strerror(errorCode)));
        }

        // Allocate message array
        *messages = new CanMessage[MAX_SIZE_MESSAGE_ARRAY];
        *messageCount = 0;

        // Allocate signal array
        *signals = new CanSignal[MAX_SIZE_SIGNAL_ARRAY];
        *signalCount = 0;

        CanMessage* message = nullptr;

        while(true)
        {
            // Read next line of the file
            errorCode = fscanf(file, "%s", dataBuffer);
            if(errorCode != 1)
            {   
                errorCode = errno;
                LOG_ERROR("Failed to parse DBC file: %s", strerror(errorCode));
                throw std::runtime_error("Failed to parse DBC file: " + std::string(strerror(errorCode)));
            }

            // Check for end of file
            if(feof(file)) break;

            // Identify keyword
            if(strcmp(dataBuffer, DBC_KEYWORD_NETWORK_NODE) == 0)
            {
                // Ignore remainder of line
                fgets(dataBuffer, SIZE_DATA_BUFFER, file);
            }
            else if(strcmp(dataBuffer, DBC_KEYWORD_MESSAGE) == 0)
            {
                if(*messageCount == MAX_SIZE_MESSAGE_ARRAY)
                {
                    LOG_ERROR("Failed to parse DBC file: The file exceeds the maximum number of messages (%i).\n", MAX_SIZE_MESSAGE_ARRAY);
                    throw std::runtime_error("Failed to parse DBC file: The file exceeds the maximum number of messages (" + std::to_string(MAX_SIZE_MESSAGE_ARRAY) + ").");
                }

                // Create data buffers
                unsigned int messageId;
                unsigned int messageDlc;

                // BO_ <id> <name>: <DLC> <Network Node>
                // - Todo: Overflow
                errorCode = fscanf(file, "%u %s %u %s", &messageId, dataBuffer, &messageDlc, dataBuffer2);

                // Remove semicolon from message name
                size_t charIndex = 0;
                while(dataBuffer[charIndex] != '\0') ++charIndex;
                if(charIndex > 0) dataBuffer[charIndex - 1] = '\0';

                // Validate input
                if(errorCode != 4)
                {
                    LOG_WARN("Failed to parse DBC message '%s': Invalid format.\n", dataBuffer);

                    // Ignore remainder of line
                    fgets(dataBuffer, SIZE_DATA_BUFFER, file);
                }
                else
                {
                    #ifdef LOG_ENTRY_PARSING
                    LOG_INFO("Message name: %s, ID: %u, DLC: %u, ECU: %s\n", dataBuffer, messageId, messageDlc, dataBuffer2);
                    #endif

                    // Add message to array
                    message = &(*messages)[*messageCount];
                    ++(*messageCount);

                    message->signalArray = *signals;
                    message->signalIndex = *signalCount;
                    message->signalCount = 0;

                    // Copy message name
                    size_t nameSize = strlen(dataBuffer);
                    message->name = new char[nameSize + 1];
                    strcpy(message->name, dataBuffer);

                    // Copy message metadata
                    message->id = static_cast<uint16_t>(messageId);
                }
            }
            else if(strcmp(dataBuffer, DBC_KEYWORD_SIGNAL) == 0)
            {
                if(message == nullptr)
                {
                    LOG_ERROR("Failed to parse DBC file: Read a signal before reading the first message.\n");
                    throw std::runtime_error("Failed to parse DBC file: Read a signal before reading the first message.");
                }

                if(*signalCount == MAX_SIZE_SIGNAL_ARRAY)
                {
                    LOG_ERROR("Failed to parse DBC file: The file exceeds the maximum number of signals (%i).\n", MAX_SIZE_SIGNAL_ARRAY);
                    throw std::runtime_error("Failed to parse DBC file: The file exceeds the maximum number of signals (" + std::to_string(MAX_SIZE_SIGNAL_ARRAY) + ").");
                }
                
                // Create data buffers
                unsigned int bitPosition;
                unsigned int bitLength;
                unsigned int endianness;
                char signedness;
                double scaleFactor;
                double offset;
                double min;
                double max;

                // Format: SG_ <Name> : <Bit position>|<Bit length>@<Endianness><Signedness> (<Scale factor>,<Offset>) [<Min>|<Max>] "<Unit>" <Network Node>
                errorCode = fscanf(file, "%s : %u|%u@%u%c (%lf,%lf) [%lf|%lf] \"%s %s", dataBuffer, &bitPosition, &bitLength, &endianness, &signedness, &scaleFactor, &offset, &min, &max, dataBuffer2, dataBuffer3);

                // Remove ending quote from unit name
                size_t charIndex = 0;
                while(dataBuffer2[charIndex] != '\0') ++charIndex;
                if(charIndex > 0) dataBuffer2[charIndex - 1] = '\0';

                // Validate input
                if(errorCode != 11)
                {
                    LOG_WARN("Failed to parse DBC signal '%s': Invalid format.\n", dataBuffer);
                    
                    // Ignore remainder of line
                    fgets(dataBuffer, SIZE_DATA_BUFFER, file);
                }
                else
                {
                    #ifdef LOG_ENTRY_PARSING
                    LOG_INFO("Signal name: %s, Bit Pos: %i, Bit Len: %i, Endianness: %i, Sign: %c, Scale: %f, Offset: %f, Min: %f, Max: %f, Unit: %s, ECU: %s\n", dataBuffer, bitPosition, bitLength, endianness, signedness, scaleFactor, offset, min, max, dataBuffer2, dataBuffer3);
                    #endif

                    // Add signal to array
                    CanSignal* signal = &(*signals)[*signalCount];
                    ++(*signalCount);
                    ++message->signalCount;

                    signal->messageArray = (*messages);
                    signal->messageIndex = *messageCount - 1;
                    
                    // Copy signal name
                    size_t nameSize = strlen(dataBuffer);
                    signal->name = new char[nameSize + 1];
                    strcpy(signal->name, dataBuffer);

                    // Copy signal metadata
                    signal->bitPosition = static_cast<uint16_t>(bitPosition);
                    signal->bitLength   = static_cast<uint16_t>(bitLength);
                    signal->scaleFactor = scaleFactor;
                    signal->offset      = offset;
                    signal->signedness  = signedness == '-';

                    // Populate bitmask
                    signal->bitMask = 0;
                    for(uint16_t index = 0; index < signal->bitLength; ++index)
                    {
                        signal->bitMask |= (1 << index);
                    }

                    // Determine if signal is floating point or not
                    bool integerOffset      = (trunc(signal->offset)      == signal->offset);
                    bool integerScaleFactor = (trunc(signal->scaleFactor) == signal->scaleFactor);

                    // Determine datatype ID
                    if(integerOffset && integerScaleFactor)
                    {
                        // Integer
                        if(signal->bitLength == 1)
                        {
                            // Boolean
                            signal->datatypeId = ID_DATATYPE_BOOL;
                        }
                        else if(signal->signedness == true)
                        {
                            // Signed integer
                            signal->datatypeId = ID_DATATYPE_INT;
                        }
                        else
                        {
                            // Unsigned integer
                            signal->datatypeId = ID_DATATYPE_UINT;
                        }
                    }
                    else
                    {
                        // Floating point
                        signal->datatypeId = ID_DATATYPE_DOUBLE;
                    }
                }
            }
            else if(strcmp(dataBuffer, DBC_KEYWORD_ENV_VARIABLE) == 0)
            {
                // TODO: Figure out format
                LOG_WARN("Environment variables are not implemented. Ignoring...\n");
            }
            else if(strcmp(dataBuffer, DBC_KEYWORD_SIG_GROUP) == 0)
            {
                // TODO: Figure out format
                LOG_WARN("Signal groups are not implemented. Ignoring...\n");
            }
            else if(strcmp(dataBuffer, DBC_KEYWORD_VAL_TABLE) == 0)
            {
                LOG_WARN("Value tables are not implemented. Ignoring...\n");

                // Ignore remainder of line
                fgets(dataBuffer, SIZE_DATA_BUFFER, file);
            }
            else if(strcmp(dataBuffer, DBC_KEYWORD_VERSION) == 0)
            {
                // Ignore remainder of line
                fgets(dataBuffer, SIZE_DATA_BUFFER, file);
            }
            else if(strcmp(dataBuffer, DBC_KEYWORD_BIT_TIMING) == 0)
            {
                // Ignore remainder of line
                fgets(dataBuffer, SIZE_DATA_BUFFER, file);
            }
            else if(strcmp(dataBuffer, DBC_KEYWORD_COMMENT) == 0)
            {
                // Ignore remainder of line
                fgets(dataBuffer, SIZE_DATA_BUFFER, file);
            }
            else if(strcmp(dataBuffer, DBC_KEYWORD_NS) == 0)
            {
                // Ignore every following line starting with whitespace
                while(true)
                {
                    // Read first char and check to see if it is whitespace
                    errorCode = fgetc(file);
                    if(errorCode == EOF) break;
                    unsigned char startingChar = static_cast<unsigned char>(errorCode);
                    if(startingChar != ' ' && startingChar != '\t')
                    {
                        // Return the stream to its previous state and return to normal loop
                        ungetc(static_cast<int>(startingChar), file);
                        break;
                    }

                    // Ignore remainder of line
                    char* errorChar = fgets(dataBuffer, SIZE_DATA_BUFFER, file);
                    if(errorChar == NULL) break;
                }
            }
            else
            {
                LOG_WARN("Unknown keyword '%s'. Ignoring line...\n", dataBuffer);

                // Ignore remainder of line
                fgets(dataBuffer, SIZE_DATA_BUFFER, file);
            }

            // Check for end of file
            if(feof(file)) break;
        }

        // Close the file
        if(file != NULL)
        {
            fclose(file);
        }

        // Shrink the arrays to save memory, not necessary but should be done
        reallocateMessages(signals, messages, MAX_SIZE_SIGNAL_ARRAY, *signalCount, MAX_SIZE_MESSAGE_ARRAY, *messageCount);
    }
}