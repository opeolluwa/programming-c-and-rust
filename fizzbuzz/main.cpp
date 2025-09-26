#include <iostream>

int main()
{
    int target{};

    std::cout << "Input the target value: " << std::endl;
    std::cin >> target;

    if (target <= 0)
    {
        std::cerr << "Err! Please use a non negative value";
        return EXIT_FAILURE;
    }

    for (int counter = 0; counter < target; counter++)
    {
        if (counter % 5 == 0 && counter % 3 == 0)
        {
            std::cout << "FizzBuzz" << std::endl;
        }
        else if (counter % 5 == 0)
        {
            std::cout << "Fizz" << std::endl;
        }
        else if (counter % 3 == 0)
        {
            std::cout << "Buzz" << std::endl;
        }
    }
}
