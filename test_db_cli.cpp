#include "database.h"

#include <iostream>

#include <cstdlib>

int main(int argc, char **argv)
{
    // Require single argument
    if(argc != 2)
    {
        std::cerr << "Invalid format. Usage: database_cli <database size>" << std::endl;
        return 1;
    }

    // Read database size
    unsigned int size = atoi(argv[1]);

    Database db(size);

    while(true)
    {
        char option;

        std::string key;

        std::cout << "Select an option:"       << std::endl
                  << "  i - Read an integer"   << std::endl
                  << "  c - Read a char"       << std::endl
                  << "  b - Read a bool"       << std::endl
                  << "  n - Insert an integer" << std::endl
                  << "  h - Insert a char"     << std::endl
                  << "  o - Insert a bool"     << std::endl
                  << "  q - Quit"              << std::endl;

        std::cin >> option;

        std::cout << "Enter the key: ";

        std::cin >> key;

        // Because switches are stupid
        int*  rInt;
        char* rChar;
        bool* rBool;
        int   iInt;
        char  iChar;
        bool  iBool;

        switch(option)
        {
            case 'i':
                rInt = db.reference<int>(key);
                std::cout << "Read: " << *rInt << std::endl;
                continue;
            case 'c':
                rChar = db.reference<char>(key);
                std::cout << "Read: " << *rChar << std::endl;
                continue;
            case 'b':
                rBool = db.reference<bool>(key);
                std::cout << "Read: " << *rBool << std::endl;
                continue;
            case 'n':
                iInt;
                std::cout << "Enter the value: ";
                std::cin >> iInt;
                db.insert(iInt, key);
                std::cout << "Inserted." << std::endl;
                continue;
            case 'h':
                iChar;
                std::cout << "Enter the value: ";
                std::cin >> iChar;
                db.insert(iChar, key);
                std::cout << "Inserted." << std::endl;
                continue;
            case 'o':
                iBool;
                std::cout << "Enter the value: ";
                std::cin >> iBool;
                db.insert(iBool, key);
                std::cout << "Inserted." << std::endl;
                continue;
            case 'q':
                return 0;
        }
        
        std::cerr << "Enter a valid option." << std::endl;
    }
}