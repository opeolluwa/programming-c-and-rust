#include <iostream>


std::string get_friend_full_name()
{
    std::string first_name;
    std::string last_name;
    std::cout << "First name?:\t";
    std::cin >> first_name;
    std::cout << "Last name?\t:";
    std::cin >> last_name;


    std::string full_name = first_name + " " + last_name;
    return full_name;
}


int main()
{
    std::cout << "Hello, World!" << std::endl;


    std::vector<std::string> friends{};
    int friends_count{};

    std::cout << "How many friends you've got?:\t";
    std::cin >> friends_count;

    for (int count{}; count < friends_count; count++)
    {
        std::string full_name = get_friend_full_name();
        friends.push_back(full_name);
    }

    std::cout << "Your friends are\n:";
    for (const auto& name : friends)
    {
        std::cout << "-" << name << "\n";
    }
}
