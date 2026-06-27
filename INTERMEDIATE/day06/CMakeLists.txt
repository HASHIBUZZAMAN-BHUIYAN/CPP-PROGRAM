cmake_minimum_required(VERSION 3.16)
project(Day06_CMakeIntro CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# The main executable
add_executable(day06_app lesson.cpp)

# Tell CMake to look for headers in the current source directory
target_include_directories(day06_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Compiler warnings
target_compile_options(day06_app PRIVATE -Wall -Wextra)

message(STATUS "Configuring Day 06 CMake intro project")
message(STATUS "  Source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "  Build dir:  ${CMAKE_CURRENT_BINARY_DIR}")
