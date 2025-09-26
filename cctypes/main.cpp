#include <iostream>
#include <cctype>

int main()
{
    char user_input{};
    std::cout << "Please provide an input " << std::endl;

    std::cin >> user_input;

    if (std::isupper(user_input))
    {
        std::cout << "You entered an upper digit " << user_input << "\n";
    }
    else if (std::islower(user_input))
    {
        std::cout << "You entered an lower digit " << user_input << "\n";
    }
    else if (std::isalpha(user_input))
    {
        std::cout << "You entered an alphabet  " << user_input << "\n";
    }
    else if (std::isdigit(user_input))
    {
        std::cout << "You entered a digit " << user_input << "\n";
    }
    else if (std::isalnum(user_input))
    {
        std::cout << "You entered an alpha  numeric digit " << user_input << "\n";
    }else
    {
        std::cout << "You entered one of hex, punct, cntrl or graph " << user_input << "\n";
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
