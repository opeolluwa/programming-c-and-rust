#include <iostream>
#include <array>

int main()
{
    std::array<unsigned, 5> scores{40, 78, 34, 67, 90};

    for (auto score : scores)
    {
        std::cout << score << std::endl;
    }

    std::cout << scores.size() << std::endl;
    return 0;
}
