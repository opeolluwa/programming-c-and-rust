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

void createTodo(sqlite3* db);
void makeDone(sqlite3* db);
void deleteTodo(sqlite3* db);
void updateTodo(sqlite3* db);
void selectOneTodo(sqlite3* db);
void listTodos(sqlite3* db);
std::optional<Todo> findTodo(sqlite3* db, const std::string& todoIdentifier);
std::optional<std::string> getIdentifier();

void processSelection(const Command& command, sqlite3* db);

int main(int argc, char* argv[])
{
    sqlite3* db;
    const std::string createTableSql = "CREATE TABLE IF NOT EXISTS"
        " todos ("
        "identifier TEXT PRIMARY KEY NOT NULL,"
        "title TEXT NOT NULL,"
        "description TEXT,"
        "done INTEGER NOT NULL DEFAULT 0);";

    int exitCode = 0;
    exitCode = sqlite3_open("todo.sqlite", &db);
    char* errorMessage;
    exitCode = sqlite3_exec(db, createTableSql.c_str(), NULL, 0, &errorMessage);

    if (exitCode != SQLITE_OK)
    {
        std::cerr << "Error creating table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
    else
        std::cout << "Table created successfully" << std::endl;

    try
    {
        auto options = structopt::app("todo").parse<CommandLineOptions>(argc, argv);
        std::string commandPassed = options.command;

        auto command = magic_enum::enum_cast<Command>(commandPassed);
        if (command.has_value())
        {
            processSelection(command.value(), db);
        }
    }
    catch (structopt::exception& e)
    {
        std::cout << e.what() << "\n";
        std::cout << e.help();
        sqlite3_close(db);
    }
    sqlite3_close(db);
}

void processSelection(const Command& command, sqlite3* db)
{
    switch (command)
    {
    case Command::CreateTodo:
        createTodo(db);
        break;
    case Command::DeleteTodo:
        deleteTodo(db);
        break;
    case Command::UpdateTodo:
        updateTodo(db);
        break;
    case Command::MarkDone:
        makeDone(db);
        break;
    case Command::ListTodo:
        listTodos(db);
        break;
    default:
        std::cerr << "Invalid selection\n";
    }
}

void createTodo(sqlite3* db)
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
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::random_device rd;
    auto seedData = std::array<int, std::mt19937::state_size>{};
    std::generate(std::begin(seedData), std::end(seedData), std::ref(rd));
    std::seed_seq seq(std::begin(seedData), std::end(seedData));
    std::mt19937 generator(seq);
    uuids::uuid_random_generator gen{generator};

    uuids::uuid const identifier = gen();

    sqlite3_bind_text(stmt, 1, to_string(identifier).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, description.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
}

void updateTodo(sqlite3* db)
{
    std::cout << "Update todo\n";
    std::string newTitle;
    std::string newDescription;

    std::optional<std::string> todoIdentifier = getIdentifier();

    if (!todoIdentifier)
    {
        std::cout << "Looks like you entered an incorrect identifier\n";
    }
    const std::string& identifier = *todoIdentifier;

    std::cout << "Type the new title or press the Enter key to skip update:\n";
    std::getline(std::cin, newTitle);

    std::cout << "Type the new description or press the Enter key to skip update:\n";
    std::getline(std::cin, newDescription);

    const bool hasTitleUpdate = !newTitle.empty();
    const bool hasDescriptionUpdate = !newDescription.empty();

    if (!hasDescriptionUpdate && !hasTitleUpdate)
    {
        std::cout << "No updates provided exciting...";
        return;
    }

    std::string sql;
    if (hasTitleUpdate && hasDescriptionUpdate)
        sql = "UPDATE todos SET title = ?, description = ? WHERE identifier = ?";
    else if (hasTitleUpdate)
        sql = "UPDATE todos SET title = ? WHERE identifier = ?";
    else
        sql = "UPDATE todos SET description = ? WHERE identifier = ?";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    int paramIndex = 1;
    if (hasTitleUpdate)
        sqlite3_bind_text(stmt, paramIndex++, newTitle.c_str(), -1, SQLITE_TRANSIENT);

    if (hasDescriptionUpdate)
        sqlite3_bind_text(stmt, paramIndex++, newDescription.c_str(), -1, SQLITE_TRANSIENT);

    sqlite3_bind_text(stmt, paramIndex, identifier.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE)
        std::cout << "Todo updated successfully.\n";
    else
        std::cerr << "Update failed: " << sqlite3_errmsg(db) << "\n";

    sqlite3_finalize(stmt);
}

void deleteTodo(sqlite3* db)
{
    std::cout << "Delete todo\n\n";

    std::optional<std::string> todoIdentifier = getIdentifier();
    if (!todoIdentifier)
    {
        std::cout << "Looks like you entered an incorrect identifier\n";
        return;
    }

    const std::string& identifier = *todoIdentifier;

    // Confirm deletion
    std::cout << "Are you sure you want to delete this todo? (y/n): ";
    std::string confirm;
    std::getline(std::cin, confirm);

    if (confirm != "y" && confirm != "Y")
    {
        std::cout << "Deletion cancelled.\n";
        return;
    }

    sqlite3_stmt* stmt = nullptr;
    const char* sql = "DELETE FROM todos WHERE identifier = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare delete statement: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, identifier.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE)
    {
        std::cout << "Todo deleted successfully.\n";
    }
    else
    {
        std::cerr << "Failed to delete todo: " << sqlite3_errmsg(db) << "\n";
    }

    sqlite3_finalize(stmt);
}


void makeDone(sqlite3* db)
{
    std::cout << "Mark todo as done\n\n";

    std::optional<std::string> todoIdentifier = getIdentifier();
    if (!todoIdentifier)
    {
        std::cout << "Looks like you entered an incorrect identifier\n";
        return;
    }

    const std::string& identifier = *todoIdentifier;

    // Optional: check if todo exists
    auto todo = findTodo(db, identifier);
    if (!todo)
    {
        std::cout << "Todo not found.\n";
        return;
    }

    if (todo->done)
    {
        std::cout << "Todo is already marked as done.\n";
        return;
    }

    sqlite3_stmt* stmt = nullptr;
    const char* sql = "UPDATE todos SET done = 1 WHERE identifier = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, identifier.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE)
    {
        std::cout << "Todo marked as done successfully.\n";
    }
    else
    {
        std::cerr << "Failed to mark todo as done: " << sqlite3_errmsg(db) << "\n";
    }

    sqlite3_finalize(stmt);
}


void listTodos(sqlite3* db)
{
    sqlite3_stmt* stmt = nullptr;
    const auto sql = "SELECT identifier, title, description FROM todos";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
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
        std::cerr << "Error while reading data: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}


std::optional<Todo> findTodo(sqlite3* db, const std::string& todoIdentifier)
{
    sqlite3_stmt* stmt = nullptr;
    const auto sql = "SELECT identifier, title, description FROM todos WHERE identifier = ?";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cout << "Failed prepared statement\n";
        return {};
    }

    sqlite3_bind_text(stmt, 1, todoIdentifier.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        Todo todo{
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
            sqlite3_column_int(stmt, 3) != 0
        };

        sqlite3_finalize(stmt);
        return todo;
    }
    if (rc == SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return {};
    }
    std::string err = sqlite3_errmsg(db);
    sqlite3_finalize(stmt);
    return {};
}


std::optional<std::string> getIdentifier()
{
    std::string todoIdentifier;
    std::cout << "Provide the Todo Identifier";
    std::getline(std::cin, todoIdentifier);

    if (todoIdentifier.empty() || todoIdentifier.length() < 36)
    {

        return {};
    }

    return todoIdentifier;
}
