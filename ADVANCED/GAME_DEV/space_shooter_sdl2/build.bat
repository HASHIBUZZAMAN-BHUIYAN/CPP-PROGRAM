@echo off
set SDL2=C:\Users\zen\Documents\GitHub\C-PROGRAM\SDL2\SDL2-2.32.8\x86_64-w64-mingw32

echo Building space_shooter...
g++ -std=c++17 -Wall -O2 -o space_shooter.exe space_shooter.cpp ^
    -I"%SDL2%\include" ^
    -L"%SDL2%\lib" ^
    -lmingw32 -lSDL2main -lSDL2 -mwindows

if %errorlevel% neq 0 (
    echo BUILD FAILED
    exit /b 1
)

if not exist SDL2.dll (
    copy "%SDL2%\bin\SDL2.dll" . >nul
    echo Copied SDL2.dll
)

echo OK - Run: space_shooter.exe
