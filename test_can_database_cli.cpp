#include "can_database.h"

#include <iostream>

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        std::cerr << "Invalid format. Usage: ./test_can_database_cli <can device name> <database file name>" << std::endl;
    }

    std::string deviceName = argv[1];
    std::string dbcFileName = argv[2];

    CanDatabase db(dbcFileName, deviceName);

    CanSocket txRxSocket(deviceName);

    while(true)
    {
        char option;

        std::string key;

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
            std::cout << "Enter the ID of the message (hex): ";
            std::cin >> std::hex >> id;
            
            uint64_t data = 0;
            std::string dataBuffer;
            unsigned int dataCount;

            std::cout << "Enter the bytes of the message (hex): ";
            
            std::cin >> dataBuffer;

            dataCount = dataBuffer.length() / 2;
            
            for(size_t index = 0; index < dataCount; ++index)
            {
                switch(dataBuffer[dataBuffer.length() - index - 1])
                {
                case '0':
                    data |= 0 << (4 * index);
                    break;
                case '1':  
                    data |= 1 << (4 * index);
                    break;
                case '2':
                    data |= 2 << (4 * index);
                    break;
                case '3':
                    data |= 3 << (4 * index);
                    break;
                case '4':
                    data |= 4 << (4 * index);
                    break;
                case '5':
                    data |= 5 << (4 * index);
                    break;
                case '6':
                    data |= 6 << (4 * index);
                    break;
                case '7':
                    data |= 7 << (4 * index);
                    break;
                case '8':
                    data |= 8 << (4 * index);
                    break;
                case '9':
                    data |= 9 << (4 * index);
                    break;
                case 'A':
                case 'a':
                    data |= 10 << (4 * index);
                    break;
                case 'B':
                case 'b':
                    data |= 11 << (4 * index);
                    break;
                case 'C':
                case 'c':
                    data |= 12 << (4 * index);
                    break;
                case 'D':
                case 'd':
                    data |= 13 << (4 * index);
                    break;
                case 'E':
                case 'e':
                    data |= 14 << (4 * index);
                    break;
                case 'F':
                case 'f':
                    data |= 15 << (4 * index);
                    break;
                default:
                    std::cerr << "Invalid character." << std::endl;
                    continue;
                }
            }

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
}