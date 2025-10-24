#include <iostream>

void by_value(int);
void by_reference(int&);

int main()
{
    int age{45};
    by_value(age);

    by_reference(age);

    // by_value(age);
}


void by_value(int age)
{
    age *= 5;
    std::cout << "The new age is " << age << std::endl;
}

void by_reference(int& age)
{
    age *= -3;

    std::cout << "The new age is " << age << std::endl;
}
