// ZRE Dashboard TUI ----------------------------------------------------------------------------------------------------------
//
// Author: Cole Barach
//
// Description: A TUI (text-based user interface) for the ZRE dashboard backend. Used to test and maintain the libraries used
//   by the dashboard's GUI.
//
// Created: 23.07.30
// Updated: 23.08.03
//
// To do:
// - The database RX thread segfaults.
// - Figure out scrolling
// - Make it look nicer

// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "can_database.h"

// NCurses Libraries
#include <curses.h>

// C Standard Libraries
#include <unistd.h>
#include <string.h>

// C++ Standard Libraries
#include <string>
#include <iostream>

// Function Prototypes --------------------------------------------------------------------------------------------------------

bool handleArguments(int argc, char** argv, std::string* deviceName, std::string* dbcFileName);

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
	std::string dbcPath;

	bool isValid = handleArguments(argc, argv, &deviceName, &dbcPath);
	if(!isValid) return 0;

	Network::CanDatabase database(dbcPath.c_str(), deviceName.c_str());

    initscr();

    // TODO: Doesn't work with move?
    scrollok(stdscr, true); // Enable scrolling outside window
    idlok(stdscr, true);    // Enable terminal scrolling

    const Network::CanMessage* messages     = database.getMessages();
    size_t                     messageCount = database.getMessageCount();

    for(int index = 0; index < messageCount + database.getSignalCount(); ++index)
    {
        printw("%d\n", index);
    }

    // mvaddstr(0, 0,   "  Signal Name");
    // mvaddstr(0, 38,  "| Value");
    // mvaddstr(0, 51,  "| Bit Mask");
    // mvaddstr(0, 62,  "| Bit Position");
    // mvaddstr(0, 78,  "| Bit Length");
    // mvaddstr(0, 91, "| Type");

    // int line = 1;

    // for(size_t mIndex = 0; mIndex < messageCount; ++mIndex)
    // {
    //     const Network::CanMessage& m = messages[mIndex];

    //     mvaddstr(line, 0, m.name);
    //     ++line;

    //     for(size_t sIndex = m.signalIndex; sIndex <  m.signalCount + m.signalIndex; ++sIndex)
    //     {
    //         const Network::CanSignal& s = m.signalArray[sIndex];

    //         char data[64];

    //         mvaddstr(line, 0, s.name);

    //         if(s.datatypeId == ID_DATATYPE_UINT)
    //         {
    //             sprintf(data, "%d", database.get<unsigned int>(s.name));
    //             mvaddstr(line, 40, data);
    //         }
    //         else if(s.datatypeId == ID_DATATYPE_INT)
    //         {
    //             sprintf(data, "%d", database.get<int>(s.name));
    //             mvaddstr(line, 40, data);
    //         }
    //         else if(s.datatypeId == ID_DATATYPE_BOOL)
    //         {
    //             sprintf(data, "%d", database.get<bool>(s.name));
    //             mvaddstr(line, 40, data);
    //         }
    //         else if(s.datatypeId == ID_DATATYPE_DOUBLE)
    //         {
    //             sprintf(data, "%d", database.get<double>(s.name));
    //             mvaddstr(line, 40, data);
    //         }
    //         else
    //         {
    //             mvaddstr(line, 40, "--");
    //         }
            
    //         sprintf(data, "%X", s.bitMask);
    //         mvaddstr(line, 53, data);
            
    //         sprintf(data, "%X", s.bitPosition);
    //         mvaddstr(line, 64, data);
            
    //         sprintf(data, "%d", s.bitLength);
    //         mvaddstr(line, 80, data);
            
    //         if(s.datatypeId == ID_DATATYPE_UINT)
    //         {
    //             mvaddstr(line, 93, "UINT");
    //         }
    //         else if(s.datatypeId == ID_DATATYPE_INT)
    //         {
    //             mvaddstr(line, 93, "INT");
    //         }
    //         else if(s.datatypeId == ID_DATATYPE_BOOL)
    //         {
    //             mvaddstr(line, 93, "BOOL");
    //         }
    //         else if(s.datatypeId == ID_DATATYPE_DOUBLE)
    //         {
    //             mvaddstr(line, 93, "DOUBLE");
    //         }
    //         else
    //         {
    //             mvaddstr(line, 93, "--");
    //         }
            
    //         ++line;
    //     }
    // }

	// while(true)
	// {
    //     line = 1;

    //     for(size_t mIndex = 0; mIndex < messageCount; ++mIndex)
    //     {
    //         const Network::CanMessage& m = messages[mIndex];

    //         ++line;

    //         for(size_t sIndex = m.signalIndex; sIndex <  m.signalCount + m.signalIndex; ++sIndex)
    //         {
    //             const Network::CanSignal& s = m.signalArray[sIndex];

    //             char data[64];

    //             if(s.datatypeId == ID_DATATYPE_UINT)
    //             {
    //                 sprintf(data, "%d", database.get<unsigned int>(s.name));
    //                 mvaddstr(line, 40, data);
    //             }
    //             else if(s.datatypeId == ID_DATATYPE_INT)
    //             {
    //                 sprintf(data, "%d", database.get<int>(s.name));
    //                 mvaddstr(line, 40, data);
    //             }
    //             else if(s.datatypeId == ID_DATATYPE_BOOL)
    //             {
    //                 sprintf(data, "%d", database.get<bool>(s.name));
    //                 mvaddstr(line, 40, data);
    //             }
    //             else if(s.datatypeId == ID_DATATYPE_DOUBLE)
    //             {
    //                 sprintf(data, "%d", database.get<double>(s.name));
    //                 mvaddstr(line, 40, data);
    //             }
    //             else
    //             {
    //                 mvaddstr(line, 40, "--");
    //             }
                
    //             ++line;
    //         }
    //     }

	// 	refresh();
	// }

    while(true) refresh();

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

void printHelp()
{
    std::cout <<                                                                      std::endl
              << "Dashboard TUI - Help ------------------------------------------" << std::endl
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
              << "Dashboard TUI - Tutorial ---------------------------------------" << std::endl
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
    std::cout << "Usage: dash-tui [options] <device name> <dbd file name>" << std::endl;
}