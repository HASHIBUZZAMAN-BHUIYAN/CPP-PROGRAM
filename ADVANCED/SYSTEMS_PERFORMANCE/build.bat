@echo off
echo Building ADVANCED SYSTEMS_PERFORMANCE programs...
echo.

echo [1/5] custom_memory_pool.cpp...
g++ -std=c++17 -Wall -O2 -o custom_memory_pool.exe custom_memory_pool.cpp
if %errorlevel% neq 0 (echo FAIL) else (echo OK)

echo [2/5] raii_resource_management.cpp...
g++ -std=c++17 -Wall -O2 -o raii_resource_management.exe raii_resource_management.cpp
if %errorlevel% neq 0 (echo FAIL) else (echo OK)

echo [3/5] move_semantics_optimization.cpp...
g++ -std=c++17 -Wall -O2 -o move_semantics_optimization.exe move_semantics_optimization.cpp
if %errorlevel% neq 0 (echo FAIL) else (echo OK)

echo [4/5] multithreading_modern_cpp.cpp...
g++ -std=c++17 -Wall -O2 -o multithreading_modern_cpp.exe multithreading_modern_cpp.cpp -lpthread
if %errorlevel% neq 0 (echo FAIL) else (echo OK)

echo [5/5] profiling_and_benchmarking.cpp...
g++ -std=c++17 -Wall -O2 -o profiling_and_benchmarking.exe profiling_and_benchmarking.cpp
if %errorlevel% neq 0 (echo FAIL) else (echo OK)

echo.
echo All built. Run each .exe to see output.
