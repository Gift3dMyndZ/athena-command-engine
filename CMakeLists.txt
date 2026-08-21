cmake_minimum_required(VERSION 3.25)

project(
    athena_command_engine
    VERSION 0.1.0
    DESCRIPTION "C++ AI intelligence service for the Platform Engineering Command Center"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
    athena
    src/main.cpp
)

target_compile_options(
    athena
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)