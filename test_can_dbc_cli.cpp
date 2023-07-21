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
    
    std::vector<CanDbc::CanMessage> messages;
    
    CanDbc::parseFile(dbcFile, messages);

    std::cout << "Database Table:" << std::endl;
    std::cout << messages << std::endl;
    
    return 0;
}