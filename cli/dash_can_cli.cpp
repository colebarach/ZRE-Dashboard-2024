// Dashboard CAN Command-line Interface ---------------------------------------------------------------------------------------
//
// Author: Cole Barach
//
// Description: A command-line tool for developing and testing the dashboard libraries.
//
// Created: 23.07.08
// Updated: 23.09.04

// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "can_database.h"

// C Standard Libraries
#include <string.h>

// C++ Standard Libraries
#include <iostream>

// Function Prototypes --------------------------------------------------------------------------------------------------------

bool handleArguments(int argc, char** argv, std::string* deviceName, std::string* dbcFileName);

void handleMenu(Network::CanDatabase& db, Network::CanSocket& txRxSocket);

void handleExtendedMenu(Network::CanDatabase& db, Network::CanSocket& txRxSocket);

void printHelp();

void printTutorial();

void printFormat();

// Arguments ------------------------------------------------------------------------------------------------------------------

#define OPTION_STRING_HELP     "help"
#define OPTION_STRING_TUTORIAL "tutorial"

#define OPTION_CHAR_HELP 'h'

// Constants ------------------------------------------------------------------------------------------------------------------

#define SIZE_DATABASE_TABLE_BUFFER 16384

// Entrypoint -----------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    std::string deviceName;
    std::string dbcFileName;

    bool isValid = handleArguments(argc, argv, &deviceName, &dbcFileName);
    if(!isValid) return 0;

    Network::CanDatabase database(dbcFileName.c_str(), deviceName.c_str());
    Network::CanSocket   rawSocket(deviceName.c_str());

    while(true)
    {
        try
        {
            handleMenu(database, rawSocket);
        }
        catch(const std::exception& exception)
        {
            std::cerr << "Error: " << exception.what() << std::endl;
        }
    }

    return 1;
}

// Menus ----------------------------------------------------------------------------------------------------------------------

bool handleArguments(int argc, char** argv, std::string* deviceName, std::string* dbcFileName)
{
    bool deviceNameFound = false;
    bool dbcFileNameFound = false;

    for(int index = 1; index < argc; ++index)
    {
        if(argv[index][0] == '-')
        {
            if(argv[index][1] == '-')
            {
                // Option string
                if(strcmp(argv[index] + 2, OPTION_STRING_HELP) == 0)
                {
                    printHelp();
                    return false;
                }
                else if(strcmp(argv[index] + 2, OPTION_STRING_TUTORIAL) == 0)
                {
                    printTutorial();
                    return false;
                }
                else
                {
                    std::cout << "Unknown option \"" << argv[index] + 2 << "\"." << std::endl;
                    return false;
                }
            }
            else
            {
                // Option character
                if(argv[index][1] == OPTION_CHAR_HELP)
                {
                    printHelp();
                    return false;
                }
                else
                {
                    std::cout << "Unknown option '" << argv[index][1] << "'." << std::endl;
                    return false;
                }
            }
        }
        else
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
            else
            {
                std::cout << "Unknown argument \"" << argv[index] << "\"." << std::endl;
                printFormat();
                return false;
            }
        }
    }

    if(deviceNameFound && dbcFileNameFound) return true;

    printFormat();
    return false;
}

void handleMenu(Network::CanDatabase& database, Network::CanSocket& rawSocket)
{
    char option;
    std::cout << "Select an option:"              << std::endl
              << "  u - Read an unsigned integer" << std::endl
              << "  i - Read an integer"          << std::endl
              << "  b - Read a bool"              << std::endl
              << "  d - Read a double"            << std::endl
              << "  n - Send an unsigned integer" << std::endl
              << "  j - Send an integer"          << std::endl
              << "  l - Send a bool"              << std::endl
              << "  e - Send a double"            << std::endl
              << "  p - Print the database"       << std::endl
              << "  c - Next menu"                << std::endl
              << "  q - Quit"                     << std::endl;
    std::cin >> option;

    if(option == 'q')
    {
        exit(0);
    }
    else if(option == 'u')
    {
        std::string key;
        std::cout << "Enter the key: ";
        std::cin >> key;

        std::cout << std::dec << "Read: " << database.get<unsigned int>(key.c_str()) << std::endl;
    }
    else if(option == 'i')
    {
        std::string key;
        std::cout << "Enter the key: ";
        std::cin >> key;

        std::cout << std::dec << "Read: " << database.get<int>(key.c_str()) << std::endl;
    }
    else if(option == 'b')
    {
        std::string key;
        std::cout << "Enter the key: ";
        std::cin >> key;

        std::cout << std::dec << "Read: " << database.get<bool>(key.c_str()) << std::endl;
    }
    else if(option == 'd')
    {
        std::string key;
        std::cout << "Enter the key: ";
        std::cin >> key;

        std::cout << std::dec << "Read: " << database.get<double>(key.c_str()) << std::endl;
    }
    else if(option == 'n')
    {
        std::string key;
        std::cout << "Enter the key: ";
        std::cin >> key;
        unsigned int data;
        std::cout << "Enter the value: ";
        std::cin >> data;

        database.send(key.c_str(), data);
        std::cout << "Sent." << std::endl;
    }
    else if(option == 'j')
    {
        std::string key;
        std::cout << "Enter the key: ";
        std::cin >> key;
        int data;
        std::cout << "Enter the value: ";
        std::cin >> data;

        database.send(key.c_str(), data);
        std::cout << "Sent." << std::endl;
    }
    else if(option == 'l')
    {
        std::string key;
        std::cout << "Enter the key: ";
        std::cin >> key;
        bool data;
        std::cout << "Enter the value: ";
        std::cin >> data;

        database.send(key.c_str(), data);
        std::cout << "Sent." << std::endl;
    }
    else if(option == 'e')
    {
        std::string key;
        std::cout << "Enter the key: ";
        std::cin >> key;
        double data;
        std::cout << "Enter the value: ";
        std::cin >> data;

        database.send(key.c_str(), data);
        std::cout << "Sent." << std::endl;
    }
    else if(option == 'p')
    {
        std::cout << "Database Table:" << std::endl;
        char tableBuffer[SIZE_DATABASE_TABLE_BUFFER];
        database.print(tableBuffer, SIZE_DATABASE_TABLE_BUFFER);
        std::cout << tableBuffer;
    }
    else if(option == 'c')
    {
        handleExtendedMenu(database, rawSocket);
    }
    else
    {
        std::cerr << "Enter a valid option." << std::endl;
        std::cin.ignore(4096, '\n'); // TODO: Replace with std::limits
        std::cin.clear();
    }
}

void handleExtendedMenu(Network::CanDatabase& database, Network::CanSocket& rawSocket)
{
    char option;

    std::cout << "Select an option:"                << std::endl
              << "  m - Read a message"             << std::endl
              << "  s - Send a message"             << std::endl
              << "  t - Toggle the RX thread"       << std::endl
              << "  d - Toggle RX thread debugging" << std::endl
              << "  b - Back"                       << std::endl;
    std::cin >> option;

    if(option == 'b')
    {
        return;
    }
    else if(option == 'm')
    {
        std::cout << "TODO" << std::endl;
    }
    else if(option == 's')
    {
        uint16_t id;
        uint64_t data;
        uint8_t  dataLength;
        
        // uint8_t cannot be used with cin. It will interpret as a char, not an int.
        unsigned int dataLengthBuffer;

        std::cout << "Enter the ID of the message (hex): ";
        std::cin >> std::hex >> id;

        std::cout << "Enter the bytes of the message (hex): ";
        std::cin >> std::hex >> data;

        std::cout << "Enter the length of the message (bytes): ";
        std::cin >> std::dec >> dataLengthBuffer;
        dataLength = static_cast<uint8_t>(dataLengthBuffer);

        if(std::cin.fail())
        {
            std::cin.ignore(4096, '\n'); // TODO: Replace with std::limits
            std::cin.clear();
            return;
        }

        rawSocket.sendMessage(&data, &dataLength, &id);

        std::cout << "Sent message." << std::endl;
    }
    else if(option == 't')
    {
        if(database.getRxThreadStatus())
        {
            database.endRxThread();
            std::cout << "Ended RX thread." << std::endl;
        }
        else
        {
            database.startRxThread();
            std::cout << "Started RX thread." << std::endl;
        }
    }
    else if(option == 'd')
    {
        if(database.getRxThreadDebug())
        {
            database.setRxThreadDebug(false);
            std::cout << "Disabled RX thread debugging." << std::endl;
        }
        else
        {
            database.setRxThreadDebug(true);
            std::cout << "Enabled RX thread debugging." << std::endl;
        }
    }
    else
    {
        std::cerr << "Enter a valid option." << std::endl;
    }
}

void printHelp()
{
    std::cout <<                                                                      std::endl
              << "Dashboard CAN CLI - Help --------------------------------------" << std::endl
              <<                                                                      std::endl;
    
    printFormat();
    
    std::cout <<                                                                      std::endl
              << "Options:"                                                        << std::endl
              << " -h / --help   => Prints this menu."                             << std::endl
              << " --tutorial    => Prints a tutorial menu."                       << std::endl
              <<                                                                      std::endl;
}

void printTutorial()
{
    std::cout <<                                                                       std::endl
              << "Dashboard CAN CLI - Tutorial -----------------------------------" << std::endl
              << "  This program uses the linux kernel CAN implementation. If you"  << std::endl
              << "  do not have a CAN device to use, you can create a virtual"      << std::endl
              << "  interface."                                                     << std::endl
              <<                                                                       std::endl
              << "Creating a Virtual CAN Interface"                                 << std::endl
              << "  Use 'sudo modprobe vcan' to load the virtual CAN kernel module" << std::endl
              << "  Use 'sudo ip link add dev vcan0 type vcan' to create a device"  << std::endl
              << "    named 'vcan0' to the network."                                << std::endl
              << "  Use 'sudo ip link set up vcan0' to bring the new device online" << std::endl
              << "  Lastly begin the program using the device name 'vcan0' to test" << std::endl
              << "  the new device. You can now send and read messages on this bus" << std::endl
              <<                                                                       std::endl;
}

void printFormat()
{
    std::cout << "Usage: dash-can-cli [options] <device name> <dbd file name>" << std::endl;
}