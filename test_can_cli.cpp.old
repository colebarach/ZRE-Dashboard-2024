// Includes
#include "can_interface.h"

// Standard Libraries
#include <iostream>
#include <string>

// Flags ----------------------------------------------------------------------------------------------------------------------

#define DEBUG_MODE

// Arguments ------------------------------------------------------------------------------------------------------------------

#define ARG_STRING_HELP     "help"
#define ARG_STRING_TUTORIAL "tutorial"

#define ARG_CHAR_HELP 'h'

// Functions ------------------------------------------------------------------------------------------------------------------

void printHelp();

void printTutorial();

// Entrypoint -----------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    std::string interfaceName = "";

    // Handle arguments, ignore program name
    for(int argIndex = 1; argIndex < argc; ++argIndex) {
        if(argv[argIndex][0] == '-')
        {
            if(argv[argIndex][1] == '-')
            {
                // Full arguments
                // Format: '--argument'
                std::string argument(&argv[argIndex][2]);

                #ifdef DEBUG_MODE
                std::cout << "Handling argument: " << argument << "..." << std::endl;
                #endif

                if(argument == ARG_STRING_HELP)
                {
                    printHelp();
                    return 0;
                }
                else if(argument == ARG_STRING_TUTORIAL)
                {
                    printTutorial();
                    return 0;
                }
                // else if(argument == ARG_STRING_3)
                // {
                //     std::cout << "Arg 3" << std::endl;
                //     continue;
                // }

                std::cerr << "Unknown option: " << argument << std::endl;
                return 1;
            }
            else
            {
                // Single arguments
                // Format: '-a'
                for(int charIndex = 1; argv[argIndex][charIndex] != '\0'; ++charIndex)
                {
                    char argument = argv[argIndex][charIndex];

                    std::cout << "Handling single argument: " << argument << "..." << std::endl;

                    if(argument == ARG_CHAR_HELP)
                    {
                        printHelp();
                        return 0;
                    }

                    std::cerr << "Unknown option: " << argument << std::endl;
                    return 1;
                }
            }
        }
        else
        {
            // Main argument
            interfaceName = argv[argIndex];
        }
    }

    if(interfaceName == "")
    {
        std::cout << "Missing device name. Use 'test_can_cli -h' for help." << std::endl;
        return 1;
    }

    CanInterface ix(interfaceName);

    while(true)
    {
        std::cout << "Select an option:"        << std::endl
                  << "  's' - Send a message"   << std::endl
                  << "  'r' - Read a message"   << std::endl
                  << "  'c' - Quit the program" << std::endl;

        char option = '\0';
        std::cin >> option;

        if(option == 's')
        {
            unsigned int id;
            std::cout << "Enter the ID of the message (hex): ";
            std::cin >> std::hex >> id;
            
            std::string dataBuffer;
            unsigned char data[8];
            unsigned int dataCount = 8;

            std::cout << "Enter the bytes of the message (hex): ";
            
            std::cin >> dataBuffer;

            dataCount = dataBuffer.length();
            
            for(int index = 0; index < dataCount; ++index)
            {
                switch(dataBuffer[index])
                {
                case '0':
                    data[index] = '\x00';
                    break;
                case '1':
                    data[index] = '\x01';
                    break;
                case '2':
                    data[index] = '\x02';
                    break;
                case '3':
                    data[index] = '\x03';
                    break;
                case '4':
                    data[index] = '\x04';
                    break;
                case '5':
                    data[index] = '\x05';
                    break;
                case '6':
                    data[index] = '\x06';
                    break;
                case '7':
                    data[index] = '\x07';
                    break;
                case '8':
                    data[index] = '\x08';
                    break;
                case '9':
                    data[index] = '\x09';
                    break;
                case 'A':
                case 'a':
                    data[index] = '\x0A';
                    break;
                case 'B':
                case 'b':
                    data[index] = '\x0B';
                    break;
                case 'C':
                case 'c':
                    data[index] = '\x0C';
                    break;
                case 'D':
                case 'd':
                    data[index] = '\x0D';
                    break;
                case 'E':
                case 'e':
                    data[index] = '\x0E';
                    break;
                case 'F':
                case 'f':
                    data[index] = '\x0F';
                    break;
                default:
                    std::cerr << "Invalid character." << std::endl;
                    continue;
                }
            }

            ix.send_m(id, data, dataCount);

            std::cout << "Sent message." << std::endl;

            continue;
        }

        if(option == 'r')
        {
            unsigned int id;
            unsigned char data[8];
            unsigned int dataBuffer;
            unsigned int dataCount; 

            std::cout << "Waiting for message..." << std::endl;

            ix.read_m(id, data, dataCount);

            std::cout << "Read ID: " << std::hex << id << ". Data: ";

            for(int index = 0; index < dataCount; ++index)
            {
                dataBuffer = static_cast<unsigned int>(data[index]);
                std::cout << std::hex << dataBuffer;
            }

            std::cout << std::endl;

            continue;
        }

        if(option == 'c')
        {
            return 0;
        }

        std::cout << "Unknown option." << std::endl;
        std::cin.clear();
        std::cin.ignore();
    }

    return 0;
}

void printHelp()
{
    std::cout <<                                                                       std::endl
              << "CAN Test CLI - Help"                                              << std::endl
              << "Usage: test_can_cli <options> <interface name>"                   << std::endl
              <<                                                                       std::endl
              << "Options:"                                                         << std::endl
              << "--help / -h - Prints this menu."                                  << std::endl
              << "--tutorial  - Prints a tutorial."                                 << std::endl
              <<                                                                       std::endl;
}

void printTutorial()
{
    std::cout <<                                                                       std::endl
              << "CAN Test CLI - Tutorial"                                          << std::endl
              << "  Use test_can_cli <device name> to begin the program with a"     << std::endl
              << "  given device. If you do not have a CAN device to use, you can"  << std::endl
              << "  create a virtual interface."                                    << std::endl
              <<                                                                       std::endl
              << "Creating a Virtual Interface"                                     << std::endl
              << "  Use 'sudo modprobe vcan' to load the virtual CAN kernel module" << std::endl
              << "  Use 'sudo ip link add dev vcan0 type vcan' to create a device"  << std::endl
              << "    named 'vcan0' to the network."                                << std::endl
              << "  Use 'sudo ip link set up vcan0' to bring the new device online" << std::endl
              << "  Lastly begin the program using the device name 'vcan0' to test" << std::endl
              << "  the new device. You can now send and read messages on this bus" << std::endl
              <<                                                                       std::endl;
}