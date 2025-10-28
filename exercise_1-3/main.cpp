/**
*exercise 1-3: the following program produces several compiler errors. Find these errors and correct them so the program can compile cleanly and run.
include <iostream>
Int main() {
std:cout << "Hello World" << std:endl
)


1. Missing "#" in include
2. Int, instead of int
3. std:cout, instead of std::cout
4. Missing ";" after statement
5. std:endl instead of std::endl
6. "(" in place of  "}"
*/

#include <iostream>

int main()
{
    std::cout << "Hello World" << std::endl;
}
