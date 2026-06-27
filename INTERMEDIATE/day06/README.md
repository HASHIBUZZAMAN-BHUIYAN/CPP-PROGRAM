# INTERMEDIATE Day 06 — Intro to CMake

## Objectives
- Understand why CMake exists and what problem it solves
- Write a `CMakeLists.txt` for a real project
- Use `target_include_directories` and `target_compile_options`
- Understand the configure → generate → build workflow

## Why CMake?
`g++ file.cpp` is fine for single files. Real projects have dozens of files, 
multiple libraries, platform differences, and IDE integration needs. 
CMake is the standard C++ build system generator used by virtually every 
large C++ project (Chromium, LLVM, OpenCV, Qt, etc.).

## Project Structure
```
day06/
├── CMakeLists.txt    <- build config
├── lesson.cpp        <- main program
├── mathlib.h         <- library header
└── README.md
```

## How to Build & Run
```bat
cmake -B build -G "MinGW Makefiles"
cmake --build build
build\day06_app.exe
```

## CMakeLists.txt Explained
```cmake
cmake_minimum_required(VERSION 3.16)   # minimum CMake version
project(MyProject CXX)                 # project name and language
set(CMAKE_CXX_STANDARD 17)             # require C++17
add_executable(myapp main.cpp)         # create executable from source files
target_include_directories(myapp PRIVATE include/)  # add include path
target_link_libraries(myapp PRIVATE mylib)          # link a library
```

## Time Estimate
~1.5 hours
