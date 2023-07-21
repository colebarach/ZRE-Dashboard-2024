// C Standard Libraries
#include <cstdint>

// C++ Standard Libraries
#include <string>
#include <vector>

// References:
// - http://mcu.so/Microcontroller/Automotive/dbc-file-format-documentation_compress.pdf

// TODO:
// - Document DBC Keywords

namespace CanDbc
{
    // Compilation Flags ----------------------------------------------------------------------------------------------------------

    #define DEBUG_GENERAL

    #define DEBUG_PARSE_ENTRIES

    // Datatype IDs ---------------------------------------------------------------------------------------------------------------

    #define ID_TYPE_INT   0
    #define ID_TYPE_UINT  1
    #define ID_TYPE_BOOL  2

    // DBC Keywords ---------------------------------------------------------------------------------------------------------------

    #define DBC_KEYWORD_ECU       "BU_:"         // Network Node
    #define DBC_KEYWORD_MESSAGE   "BO_"          // Message object
    #define DBC_KEYWORD_SIGNAL    "SG_"          // Signal object
    #define DBC_KEYWORD_VARIABLE  "EV_"          // Environment variable
    #define DBC_KEYWORD_SIG_GROUP "SIG_GROUP_"   // Signal group
    #define DBC_KEYWORD_VAL_TABLE "VAL_TABLE_"   // Value table
    #define DBC_KEYWORD_VERSION   "VERSION"      // File version
    #define DBC_KEYWORD_COMMENT   "CM_"          // Object comment, ignore line for now
    #define DBC_KEYWORD_MISC      "NS_"          // TODO: Not sure, ignore for now
    #define DBC_KEYWORD_MISC2     "BS_:"         // TODO: Not sure
    #define DBC_KEYWORD_MISC3     "BA_DEF_"      // TODO: Not sure
    #define DBC_KEYWORD_MISC4     "BA_DEF_DEF_"  // TODO: Not sure
    #define DBC_KEYWORD_MISC5     "VAL_"         // TODO: Not sure

    struct CanSignal
    {
        std::string  name;
        int          typeId;
        uint64_t     bitMask;
        uint64_t     bitPosition;
    };

    struct CanMessage
    {
        std::string  name;
        unsigned int id;
        std::vector<CanSignal> signals;
    };

    // Parse File
    // - Call to parse the contents of a DBC file into CAN Messages and Signals
    // - Returns the total number of signals from the file
    size_t parseFile(std::string filePath, std::vector<CanMessage>& messages);

    std::ostream& operator<<(std::ostream& stream, const std::vector<CanMessage>& messages);
}