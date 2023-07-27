// Includes
#include "can_dbc.h"

// Standard Libraries
#include <iostream>
#include <string>

// Entrypoint -----------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cout << "Missing dbc name. Usage: './test_can_dbc_cli <dbc_file>'" << std::endl;
        return 1;
    }

    std::string dbcFile = argv[1];
    
    Network::CanMessage* messages;
    Network::CanSignal*  signals;
    size_t messageCount;
    size_t signalCount;

    Network::CanDbc::parseFile(dbcFile, &messages, &signals, &messageCount, &signalCount);

    std::cout << "Database Table:" << std::endl;
    Network::CanDbc::printMessages(std::cout, messages, messageCount);
    
    return 0;
}