@echo off
echo Building ADVANCED TRENDING programs...
echo.

echo [1/4] modern_cpp20_features_demo.cpp (C++20)...
g++ -std=c++20 -Wall -O2 -o modern_cpp20_features_demo.exe modern_cpp20_features_demo.cpp
if %errorlevel% neq 0 (echo FAIL) else (echo OK)

echo [2/4] network_socket_basics_cpp.cpp...
g++ -std=c++17 -Wall -O2 -o network_socket_basics_cpp.exe network_socket_basics_cpp.cpp -lws2_32
if %errorlevel% neq 0 (echo FAIL) else (echo OK)

echo [3/4] json_parsing_modern_cpp.cpp...
g++ -std=c++17 -Wall -O2 -o json_parsing_modern_cpp.exe json_parsing_modern_cpp.cpp
if %errorlevel% neq 0 (echo FAIL) else (echo OK)

echo [4/4] cmake_multi_module_project (CMake)...
cd cmake_multi_module_project
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release >nul 2>&1
cmake --build build >nul 2>&1
if %errorlevel% neq 0 (echo FAIL) else (echo OK)
cd ..

echo.
echo All built.
echo Run the cmake project: cmake_multi_module_project\build\math_app.exe
echo Run its tests:         cmake_multi_module_project\build\math_tests.exe
