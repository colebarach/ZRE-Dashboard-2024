// Header
#include "can_dbc.h"

// C++ Standard Libraries
#include <fstream>
#include <iostream>
#include <iomanip>

size_t CanDbc::parseFile(std::string filePath, std::vector<CanMessage>& messages)
{
    #ifdef DEBUG_GENERAL
    std::cout << "Loading dbc file '" << filePath << "'..." << std::endl;
    #endif

    std::ifstream file(filePath, std::ios_base::in);
    std::string data;

    size_t signalCount = 0;

    size_t messageIndex = -1;
    size_t signalIndex = -1;

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
            // Add message
            messages.push_back(CanMessage());
            ++messageIndex;
            signalIndex = -1;
            CanMessage& message = messages[messageIndex];

            // Read message ID 
            file >> message.id;
            // Read message name
            file >> message.name;
            // Ignore DLC
            file >> data;
            // Ignore message sending ECU
            file >> data;
        }
        else if(data == DBC_KEYWORD_SIGNAL)
        {
            // Add signal
            messages[messageIndex].signals.push_back(CanSignal());
            ++signalIndex;
            CanSignal& signal = messages[messageIndex].signals[signalIndex];
            ++signalCount;
            
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
            size_t scaleStart  = signEnd + 2;                  // Scale starts after '('
            size_t scaleEnd    = data.find(',');               // Scale ends at ','
            size_t offsetStart = scaleEnd + 1;                 // Offset starts after ','
            size_t offsetEnd   = data.find(')');               // Offset ends at ')'
            size_t minStart    = offsetEnd + 3;                // Min starts 3 characters after offset
            size_t minEnd      = data.find('|', minStart);     // Min ends with '|', second instance of this in data
            size_t maxStart    = minEnd + 1;                   // Max starts after '|', +1 char
            size_t maxEnd      = data.find(']');               // Max ends with ']'
            size_t unitStart   = data.find('"')+1;             // Unit starts with '""
            size_t unitEnd     = data.find('"', unitStart);    // Unit ends with second instance of '"'
            size_t ecuStart    = unitEnd + 3;                  // ECU starts after unit, +3 for '"' and 2 spaces
            size_t ecuEnd      = data.length()-1;              // ECU ends with line

            #ifdef DEBUG_PARSE_ENTRIES
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
            signal.name = data.substr(nameStart, nameEnd - nameStart);

            // Read bit position
            signal.bitPosition = atoi(data.substr(bitPosStart, bitPosEnd - bitPosStart).c_str());
            
            // Read bit length
            int bitLength = atoi(data.substr(bitLenStart, bitLenEnd - bitLenStart).c_str());
            
            // Populate bitmask
            signal.bitMask = 0U;
            for(int x = 0; x < bitLength; ++x)
            {
                signal.bitMask |= 1U << x;
            }

            // Read sign ("1+" => unsigned, "1-" => signed)
            bool isSigned = data.substr(signStart, signEnd - signStart) == "1-";

            // Determine type
            if(bitLength == 1)
            {
                // Boolean
                signal.typeId = ID_TYPE_BOOL;
            }
            else if(isSigned)
            {
                // Signed int
                signal.typeId = ID_TYPE_INT;
            }
            else
            {
                // Unsigned int
                signal.typeId = ID_TYPE_UINT;
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

    return signalCount;
}

std::ostream& CanDbc::operator<<(std::ostream& stream, const std::vector<CanMessage>& messages)
{
    stream << "   " << std::setw(32) << "Signal Name";
    stream << " | " << std::setw(7)  << "Type ID";
    stream << " | " << std::setw(8)  << "Bit Mask";
    stream << " | " << std::setw(12) << "Bit Position" << std::endl;
    
    for(size_t mIndex = 0; mIndex < messages.size(); ++mIndex)
    {
        const CanMessage& m = messages[mIndex];

        stream << m.name << " - ID: " << std::hex << std::setw(3) << m.id << " " << std::setw(60 - m.name.size()) << std::setfill('-') << "-" << std::endl;
        stream << std::setfill(' ');

        for(size_t sIndex = 0; sIndex < messages[mIndex].signals.size(); ++sIndex)
        {
            const CanSignal& s = m.signals[sIndex];

            stream << "   " << std::setw(32) << s.name;
            stream << " | " << std::setw(7)  << std::dec << s.typeId;
            stream << " | " << std::setw(8)  << std::hex << s.bitMask;
            stream << " | " << std::setw(12) << std::dec << s.bitPosition << std::endl;
        }

        stream << std::endl;
    }

    return stream;
}