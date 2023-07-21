#include "can_database.h"

#include <iostream>

int hexToInt(char digit);

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        std::cerr << "Invalid format. Usage: ./test_can_database_cli <can device name> <database file name>" << std::endl;
        return 1;
    }

    std::string deviceName = argv[1];
    std::string dbcFileName = argv[2];

    CanDatabase db(dbcFileName, deviceName);

    CanSocket txRxSocket(deviceName);

    while(true)
    {
        char option;

        std::string key;

        try
        {
            std::cout << "Select an option:"        << std::endl
                      << "  i - Read an integer"    << std::endl
                      << "  b - Read a bool"        << std::endl
                      << "  s - Send a message"     << std::endl
                      << "  p - Print the database" << std::endl
                      << "  q - Quit"               << std::endl;
            std::cin >> option;

            if(option == 'q')
            {
                return 0;
            }
            else if(option == 'i')
            {
                std::cout << "Enter the key: ";
                std::cin >> key;

                int* data = db.reference<int>(key);
                std::cout << std::dec << "Read: " << *data << std::endl;
            }
            else if(option == 'b')
            {
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
            else
            {
                std::cerr << "Enter a valid option." << std::endl;
            }
        }
        catch(const std::exception& exception)
        {
            std::cerr << "Error: " << exception.what() << std::endl;
        }
    }
}

int hexToInt(char digit)
{
    switch(digit)
    {
    case '0':
        return 0;
    case '1':  
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case 'A':
    case 'a':
        return 10;
    case 'B':
    case 'b':
        return 11;
    case 'C':
    case 'c':
        return 12;
    case 'D':
    case 'd':
        return 13;
    case 'E':
    case 'e':
        return 14;
    case 'F':
    case 'f':
        return 15;
    default:
        return 0;
    }
}