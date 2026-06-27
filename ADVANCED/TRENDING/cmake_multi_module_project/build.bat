@echo off
echo Building cmake_multi_module_project...
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (echo CMAKE CONFIG FAILED && exit /b 1)
cmake --build build
if %errorlevel% neq 0 (echo BUILD FAILED && exit /b 1)
echo.
echo Build OK.
echo Run:  build\math_app.exe
echo Test: build\math_tests.exe
