#include <iostream>

enum class MaritalStatus { Single, Married, Divorced, Complicated, Widowed };

int main()
{
    MaritalStatus marital_status{MaritalStatus::Complicated};
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
