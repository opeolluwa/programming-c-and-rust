#include <iostream>
#include <string>
#include <vector>
#include <structopt/app.hpp>
#include <map>
#include <magic_enum/magic_enum.hpp>
#include <sqlite3.h>
#include "uuid.h"
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

void createTodo(sqlite3* DB);
void listTodo();
void makeDone();
void deleteTodo();
void updateTodo();
void processSelection(const Command& command, sqlite3* DB);


int main(int argc, char* argv[])
{
    sqlite3* DB;
    const std::string createTableSql = "CREATE TABLE IF NOT EXISTS"
        " todos ("
        "identifier INTEGER PRIMARY KEY NOT NULL,"
        "title TEXT NOT NULL,"
        "description TEXT,"
        "done INTEGER NOT NULL DEFAULT 0);";

    int exit = 0;
    exit = sqlite3_open("todo.sqlite", &DB);
    char* errorMessage;
    exit = sqlite3_exec(DB, createTableSql.c_str(), NULL, 0, &errorMessage);

    if (exit != SQLITE_OK)
    {
        std::cerr << "Error creating table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
    else
        std::cout << "Table created successfully" << std::endl;
    try
    {
        auto options = structopt::app("todo").parse<CommandLineOptions>(argc, argv);
        std::string command_passed = options.command;
        auto command = magic_enum::enum_cast<Command>(command_passed);
        if (command.has_value())
        {
            processSelection(command.value(), DB);
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

void processSelection(const Command& command, sqlite3* DB)
{
    switch (command)
    {
    case Command::CreateTodo:
        createTodo(DB);
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


void createTodo(sqlite3* DB)
{
    sqlite3_stmt* stmt = nullptr;

    std::cout << "Create todo:\n";
    std::string title;
    std::string description;

    std::cout << "Todo title: ";
    std::getline(std::cin, title);

    std::cout << "Todo description: ";
    std::getline(std::cin, description);

    const char* sql = "INSERT INTO todo(identifier, title, description) VALUES (?, ? , ?);";
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    uuids::uuid identifier = uuids::uuid_system_generator{}();

    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, description.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(DB) << std::endl;
    }
    sqlite3_finalize(stmt);
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
