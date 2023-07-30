#include <stdint.h>

#include <iostream>

// Notes: Apparently when you use cin / cout on a uint8_t, because it is an implemented as unsigned char, it will just read
//   and write as a single character. This is so stupid, why are such important datatypes so overlooked in C++.

int main()
{
    int buffer;
    std::cout << "Enter an integer: ";
    std::cin >> buffer;
    uint8_t i = static_cast<uint8_t>(buffer);

    std::cout << "uint8_t i = " << static_cast<int>(i) << std::endl;

    std::cout << "i > 0 = " << (i > 0) << std::endl;
    std::cout << "i < 0 = " << (i < 0) << std::endl;
    std::cout << "i == 0 = " << (i == 0) << std::endl;

    std::cout << "i > 4 = " << (i > 4) << std::endl;
    std::cout << "i < 4 = " << (i < 4) << std::endl;
    std::cout << "i == 4 = " << (i == 4) << std::endl;

    std::cout << "i > 12 = " << (i > 12) << std::endl;
    std::cout << "i < 12 = " << (i < 12) << std::endl;
    std::cout << "i == 12 = " << (i == 12) << std::endl;

    std::cout << "i > 16 = " << (i > 16) << std::endl;
    std::cout << "i < 16 = " << (i < 16) << std::endl;
    std::cout << "i == 16 = " << (i == 16) << std::endl;

    std::cout << "i == '0' = " << (i == 48) << std::endl;

    return 0;
}