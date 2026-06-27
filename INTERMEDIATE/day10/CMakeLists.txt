cmake_minimum_required(VERSION 3.16)
project(Day10_TaskManager CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library: task system
add_library(tasklib STATIC
    src/task.cpp
)
target_include_directories(tasklib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(tasklib PRIVATE -Wall -Wextra)

# Main application
add_executable(task_app src/main.cpp)
target_link_libraries(task_app PRIVATE tasklib)
target_compile_options(task_app PRIVATE -Wall)

# Tests
add_executable(task_tests tests/test_tasks.cpp)
target_link_libraries(task_tests PRIVATE tasklib)
target_compile_options(task_tests PRIVATE -Wall)

message(STATUS "Day 10 Task Manager configured")
message(STATUS "  Targets: task_app, task_tests")
