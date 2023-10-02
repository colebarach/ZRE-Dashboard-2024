// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "can_database.h"

// C Standard Libraries
#include <string.h>

// C++ Standard Libraries
#include <iostream>

// Function Prototypes --------------------------------------------------------------------------------------------------------

bool handleArguments(int argc, char** argv, std::string* deviceName, std::string* dbcFileName);

// Entrypoint -----------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    std::string deviceName;
    std::string dbcFileName;

    bool isValid = handleArguments(argc, argv, &deviceName, &dbcFileName);
    if(!isValid) return 0;

    Network::CanDatabase database(dbcFileName.c_str(), deviceName.c_str());
    
    for(int index = 0; index < 90; ++index)
    {
        std::string voltageKey = "Voltage_Cell_" + std::to_string(index);
        std::string balancingKey = "Balancing_Cell_" + std::to_string(index);

        database.send(voltageKey.c_str(), index / 100.0);
        database.send(balancingKey.c_str(), (index % 5) == 0);
    }

    for(int index = 0; index < 45; ++index)
    {
        std::string temperatureKey = "Temperature_Sensor_" + std::to_string(index);
        database.send(temperatureKey.c_str(), index + 30.0);
    }

    return 0;
}

// Menus ----------------------------------------------------------------------------------------------------------------------

bool handleArguments(int argc, char** argv, std::string* deviceName, std::string* dbcFileName)
{
    bool deviceNameFound = false;
    bool dbcFileNameFound = false;

    for(int index = 1; index < argc; ++index)
    {
        if(index == argc - 2)
        {
            // Device name
            *deviceName = argv[index];
            deviceNameFound = true;
        }
        else if(index == argc - 1)
        {
            // DBC name
            *dbcFileName = argv[index];
            dbcFileNameFound = true;
        }
    }

    if(deviceNameFound && dbcFileNameFound) return true;

    return false;
}