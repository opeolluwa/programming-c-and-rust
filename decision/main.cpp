// Ex4_01.cpp
// Comparing data values
#include <iostream>

int main()
{
    char first{};
    char second{};
    std::cout << "Enter a character: ";
    std::cin >> first;
    // Stores the first character
    // Stores the second character
    std::cout << "Enter a second character: ";
    std::cin >> second;
    std::cout << std::boolalpha;
    std::cout << "The value of the expression " << first << '<' << second
        << " is: " << (first < second) << std::endl;
    std::cout << "The value of the expression " << first << "==" << second
        << " is: " << (first == second) << std::endl;
}
