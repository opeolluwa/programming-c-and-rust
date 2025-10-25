#include <iostream>

int main()
{
    unsigned int ages[6]{23, 45, 67, 89, 2, 1,};
    std::cout << std::size(ages) << std::endl;


    for (unsigned int counter : ages)
    {
        std::cout << counter << std::endl;
    }

    std::cout << "the array is " << std::size(ages) << std::endl;

    double sizes[][3]
    {
        {3.4, 5.6, 6.7},
        {5.6, 4.7, 2.3},
        {3.0, 9.2, 1.2}
    };

    std::cout << sizes[0][0];
    for (size_t i = 0; i < std::size(sizes); i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            std::cout << " the element at [" << i << "][" << j << "] is " << sizes[i][j] << std::endl;
        }
    }
    return 0;
}
