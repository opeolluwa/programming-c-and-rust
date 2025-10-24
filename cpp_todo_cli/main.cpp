#include <iostream>
#include <string>
#include <vector>
#include <structopt/app.hpp>
#include <map>
#include <magic_enum/magic_enum.hpp>
#include <sqlite3.h>

struct Todo
{
    std::string identifier;
    std::string title;
    std::string description;
    bool done;
};

using Todos = std::vector<Todo>;

enum class Command
{
    CreateTodo,
    DeleteTodo,
    UpdateTodo,
    MarkDone
};

std::map<Command, std::string> CommandToString = {
    {Command::CreateTodo, "createTodo"},
    {Command::DeleteTodo, "deleteTodo"},
    {Command::UpdateTodo, "updateTodo"},
    {Command::MarkDone, "markTodoDone"}
};

struct CommandLineOptions
{
    std::string command;
};

STRUCTOPT(CommandLineOptions, command);

void createTodo();
void listTodo();
void makeDone();
void deleteTodo();
void updateTodo();
void processSelection(const Command& command);


int main(int argc, char* argv[])
{
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("todo.sqlite", &DB);
    if (exit)
    {
        std::cerr << "Error opening todo.sqlite " << sqlite3_errmsg(DB) << std::endl;
        return (-1);
    }
    try
    {
        auto options = structopt::app("todo").parse<CommandLineOptions>(argc, argv);
        std::string command_passed = options.command;
        auto command = magic_enum::enum_cast<Command>(command_passed);
        if (command.has_value())
        {
            processSelection(command.value());
        }
    }
    catch (structopt::exception& e)
    {
        std::cout << e.what() << "\n";
        std::cout << e.help();
        sqlite3_close(DB);
    }
    sqlite3_close(DB);
}

void processSelection(const Command& command)
{
    switch (command)
    {
    case Command::CreateTodo:
        createTodo();
        break;
    case Command::DeleteTodo:
        deleteTodo();
        break;
    case Command::UpdateTodo:
        updateTodo();
        break;
    case Command::MarkDone:
        makeDone();
        break;
    default:
        std::cerr << "Invalid selection\n";
    }
}


void createTodo()
{
    std::cout << "Create todo\n";
}

void updateTodo()
{
    std::cout << "Update todo\n";
}


void deleteTodo()
{
    std::cout << "Delete todo\n";
}

void makeDone()
{
    std::cout << "Mark todo\n";
}
