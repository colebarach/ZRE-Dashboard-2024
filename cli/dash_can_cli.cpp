// Dashboard CAN Command-line Interface ---------------------------------------------------------------------------------------
//
// Author: Cole Barach
//
// Description: A command-line tool for developing and testing the dashboard libraries.
//
// Created: 23.07.08
// Updated: 23.07.30

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

// Entrypoint -----------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    std::string deviceName;
    std::string dbcFileName;

    bool isValid = handleArguments(argc, argv, &deviceName, &dbcFileName);
    if(!isValid) return 0;

    Network::CanDatabase db(dbcFileName, deviceName);
    Network::CanSocket txRxSocket(deviceName.c_str());

    while(true)
    {
        try
        {
            handleMenu(db, txRxSocket);
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
            }
            else if(index == argc - 1)
            {
                // DBC name
                *dbcFileName = argv[index];
            }
            else
            {
                std::cout << "Unknown argument \"" << argv[index] << "\"." << std::endl;
                printFormat();
                return false;
            }
        }
    }

    return true;
}

void handleMenu(Network::CanDatabase& db, Network::CanSocket& txRxSocket)
{
    char option;
    std::cout << "Select an option:"        << std::endl
              << "  i - Read an integer"    << std::endl
              << "  b - Read a bool"        << std::endl
              << "  s - Send a message"     << std::endl
              << "  p - Print the database" << std::endl
              << "  n - Next menu"          << std::endl
              << "  q - Quit"               << std::endl;
    std::cin >> option;

    if(option == 'q')
    {
        exit(0);
    }
    else if(option == 'i')
    {
        std::string key;
        std::cout << "Enter the key: ";
        std::cin >> key;

        int* data = db.reference<int>(key.c_str());
        std::cout << std::dec << "Read: " << *data << std::endl;
    }
    else if(option == 'b')
    {
        std::string key;
        std::cout << "Enter the key: ";
        std::cin >> key;

        bool* data = db.reference<bool>(key.c_str());
        std::cout << std::dec << "Read: " << *data << std::endl;
    }
    else if(option == 's')
    {
        uint16_t id;
        uint64_t data;
        uint8_t  dataLength;
        
        // uint8_t cannot be used with cin. It will interpret as a char, not an int.
        int dataLengthBuffer;

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

        txRxSocket.sendMessage(&data, &dataLength, &id);

        std::cout << "Sent message." << std::endl;
    }
    else if(option == 'p')
    {
        std::cout << "Database Table:" << std::endl;
        db.print(std::cout);
    }
    else if(option == 'n')
    {
        handleExtendedMenu(db, txRxSocket);
    }
    else
    {
        std::cerr << "Enter a valid option." << std::endl;
        std::cin.ignore(4096, '\n'); // TODO: Replace with std::limits
        std::cin.clear();
    }
}

void handleExtendedMenu(Network::CanDatabase& db, Network::CanSocket& txRxSocket)
{
    char option;

    std::cout << "Select an option:"                << std::endl
              << "  t - Toggle the RX thread"       << std::endl
              << "  d - Toggle RX thread debugging" << std::endl
              << "  b - Back"                       << std::endl;
    std::cin >> option;

    if(option == 'b')
    {
        return;
    }
    else if(option == 't')
    {
        if(db.getRxThreadStatus())
        {
            db.endRxThread();
            std::cout << "Ended RX thread." << std::endl;
        }
        else
        {
            db.startRxThread();
            std::cout << "Started RX thread." << std::endl;
        }
    }
    else if(option == 'd')
    {
        if(db.getRxThreadDebug())
        {
            db.setRxThreadDebug(false);
            std::cout << "Disabled RX thread debugging." << std::endl;
        }
        else
        {
            db.setRxThreadDebug(true);
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