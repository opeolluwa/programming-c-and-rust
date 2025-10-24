#include <iostream>
#include <string>
#include <vector>

struct Todo
{
    std::string identifier;
    std::string title;
    std::string description;
    bool done;
};

static std::vector<Todo> Todos;

void create_todo(std::string title, std::string description);
void list_todo();
void make_done(std::string todo_identifier);
void delete_todo(std::string todo_identifier);

int main()
{
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
