// Includes
#include "can_database.h"

// C++ Standard Libraries
#include <iostream>

// Function Prototypes --------------------------------------------------------------------------------------------------------

void handleMenu(CanDatabase& db, CanSocket& txRxSocket);

void handleExtendedMenu(CanDatabase& db, CanSocket& txRxSocket);

// Entrypoint -----------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        std::cerr << "Invalid format. Usage: ./test-can-cli <can device name> <database file name>" << std::endl;
        return 1;
    }

    std::string deviceName = argv[1];
    std::string dbcFileName = argv[2];

    CanDatabase db(dbcFileName, deviceName);
    CanSocket txRxSocket(deviceName);

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
}

// Menus ----------------------------------------------------------------------------------------------------------------------

void handleMenu(CanDatabase& db, CanSocket& txRxSocket)
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

        int* data = db.reference<int>(key);
        std::cout << std::dec << "Read: " << *data << std::endl;
    }
    else if(option == 'b')
    {
        std::string key;
        std::cout << "Enter the key: ";
        std::cin >> key;

        bool* data = db.reference<bool>(key);
        std::cout << std::dec << "Read: " << *data << std::endl;
    }
    else if(option == 's')
    {
        unsigned int id;
        uint64_t data;
        unsigned int dataCount = 8;
        
        std::cout << "Enter the ID of the message (hex): ";
        std::cin >> std::hex >> id;

        std::cout << "Enter the bytes of the message (hex): ";
        std::cin >> std::hex >> data;

        std::cout << "Enter the length of the message (bytes): ";
        std::cin >> std::dec >> dataCount;

        txRxSocket.send_m(id, data, dataCount);

        std::cout << "Sent message." << std::endl;
    }
    else if(option == 'p')
    {
        std::cout << "Database Table:" << std::endl;
        std::cout << db.getMessages();
    }
    else if(option == 'n')
    {
        handleExtendedMenu(db, txRxSocket);
    }
    else
    {
        std::cerr << "Enter a valid option." << std::endl;
    }
}

void handleExtendedMenu(CanDatabase& db, CanSocket& txRxSocket)
{
    char option;
    std::string key;

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