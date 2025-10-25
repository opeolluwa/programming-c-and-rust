#include <iostream>
#include <array>

int main()
{
    std::array<unsigned, 5> scores{40, 78, 34, 67, 90};

    unsigned total_score{};
    for (auto score: scores)
    {
        total_score += score;
    }

    unsigned average_score = total_score / scores.size();

    std::cout << average_score << std::endl;
    return 0;
}
