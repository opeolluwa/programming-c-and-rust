cmake_minimum_required(VERSION 4.0)
project(cpp_todo_cli)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        structopt
        GIT_REPOSITORY https://github.com/p-ranav/structopt
        GIT_TAG v0.1.3
)

FetchContent_Declare(
        magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum
        GIT_TAG v0.9.7
)

FetchContent_MakeAvailable(structopt magic_enum)

add_executable(cpp_todo_cli main.cpp)

target_link_libraries(cpp_todo_cli PRIVATE structopt::structopt magic_enum::magic_enum)

