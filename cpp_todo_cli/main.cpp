#include <iostream>
#include <string>
#include <vector>
#include <structopt/app.hpp>
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
    MarkDone,
    ListTodo,
};


struct CommandLineOptions
{
    std::string command;
};

STRUCTOPT(CommandLineOptions, command);

void createTodo(sqlite3* DB);
void makeDone();
void deleteTodo();
void updateTodo();
void selectOneTodo();
void listTodos(sqlite3* DB);

void processSelection(const Command& command, sqlite3* DB);


int main(int argc, char* argv[])
{
    sqlite3* DB;
    const std::string createTableSql = "CREATE TABLE IF NOT EXISTS"
        " todos ("
        "identifier TEXT PRIMARY KEY NOT NULL,"
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
    case Command::ListTodo:
        listTodos(DB);
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

    const char* sql = "INSERT INTO todos(identifier, title, description) VALUES (?, ? , ?);";
    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size>{};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 generator(seq);
    uuids::uuid_random_generator gen{generator};

    uuids::uuid const identifier = gen();

    sqlite3_bind_text(stmt, 1, to_string(identifier).c_str(), -1, SQLITE_TRANSIENT);
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
    std::string new_title;
    std::string new_description;
    std::string todo_identifier;

    std::cout << "Provide the Todo Identifier";
    std::getline(std::cin, todo_identifier);

    if (!todo_identifier.data() || todo_identifier.length() < 36)
    {
        std::cout << "Looks like you entered an incorrect identifier\n";
        return;
    }


    std::cout << "Type the new title or press the Enter key to skip update:\n";
    std::getline(std::cin, new_title);

    std::cout << "Type the new description or press the Enter key to skip update:\n";
    std::getline(std::cin, new_description);

    sqlite3_stmt* stmt = nullptr;
}

void deleteTodo()
{
    std::cout << "Delete todo\n";
}

void makeDone()
{
    std::cout << "Mark todo\n";
}


void listTodos(sqlite3* DB)
{
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT identifier, title, description FROM todos";

    int rc = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cout << "Failed prepared statement\n";
        return;
    }

    std::cout << "Todo:\n:";
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const unsigned char* identifier = sqlite3_column_text(stmt, 0);
        const unsigned char* title = sqlite3_column_text(stmt, 1);
        const unsigned char* description = sqlite3_column_text(stmt, 2);
        int done = sqlite3_column_int(stmt, 3);


        std::cout << "Identifier: " << (identifier ? reinterpret_cast<const char*>(identifier) : "")
            << "\nTitle: " << (title ? reinterpret_cast<const char*>(title) : "")
            << "\nDescription: " << (description ? reinterpret_cast<const char*>(description) : "")
            << "\nStatus: " << (done ? "✅ Done" : "🕒 Pending")
            << "\n-------------------\n";
    }

    if (rc != SQLITE_DONE)
    {
        std::cerr << "Error while reading data: " << sqlite3_errmsg(DB) << std::endl;
    }

    sqlite3_finalize(stmt);
}
