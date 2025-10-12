#include <iostream>
#include <string>

int main()
{
    std::string output{"The whole world revolves around this"};

    std::cout << output << std::endl;

    // to c style string
    const char* c_style_output = output.c_str();
    std::cout << c_style_output << std::endl;

    std::string snoozing(6, 'z');
    std::cout << snoozing << std::endl;

    std::string trimmed{"Many Mickle makes a muckle", 6};
    std::cout << trimmed << std::endl;

    std::string range{output, 5, 12};
    std::cout << range << std::endl;

    std::string first_name;
    std::string last_name;

    std::cout << "Input your first name:\t ";
    std::cin >> first_name;

    std::cout << "Input your last name:\t";
    std::cin >> last_name;

    std::string full_name{first_name + " " + last_name};
    std::cout << " Your full name is " << full_name << "\n"
        << "it has a length of " << full_name.length();


    std::string password ;
    std::string confirm_password;

    std::cout <<"Enter password\n";
    std::cin >> password;
    std::cout<<"enter confirm password\n";
    std::cin >> confirm_password;


    bool equal_password {};
    equal_password = password.compare(confirm_password);

    std::cout << equal_password;
    return 0;
}
