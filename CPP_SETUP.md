# C++ Toolchain Setup

## What's Installed

| Tool | Version | Location |
|------|---------|----------|
| g++ (MinGW-W64 GCC) | 16.1.0 | C:\Users\zen\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin |
| CMake | 4.3.2 | Same MinGW bundle |
| SDL2 | 2.32.8 | Shared with C-PROGRAM repo (see below) |

## Verify Your Setup

```bat
g++ --version
cmake --version
```

Both should print version numbers. If they don't, add the MinGW bin folder to your PATH.

## SDL2 Location

SDL2 dev libraries are shared with the sibling C-PROGRAM repo:
```
C:\Users\zen\Documents\GitHub\C-PROGRAM\SDL2\SDL2-2.32.8\x86_64-w64-mingw32\
  include\SDL2\    <- headers (#include <SDL2/SDL.h>)
  lib\             <- libSDL2.a, libSDL2main.a
  bin\SDL2.dll     <- runtime DLL (copy next to your .exe before running)
```

---

## Compile Patterns

### 1. Single .cpp file (plain g++)

```bat
g++ -std=c++17 -Wall -o program.exe program.cpp
program.exe
```

For C++20 features:
```bat
g++ -std=c++20 -Wall -o program.exe program.cpp
```

### 2. CMake-based project

```bat
cmake -B build -G "MinGW Makefiles"
cmake --build build
build\program.exe
```

All `CMakeLists.txt` files in this repo are pre-configured; just run those two commands from the project folder.

### 3. SDL2 project (g++ directly)

```bat
set SDL2=C:\Users\zen\Documents\GitHub\C-PROGRAM\SDL2\SDL2-2.32.8\x86_64-w64-mingw32
g++ -std=c++17 -Wall -o game.exe game.cpp ^
    -I"%SDL2%\include" ^
    -L"%SDL2%\lib" ^
    -lmingw32 -lSDL2main -lSDL2 -mwindows
copy "%SDL2%\bin\SDL2.dll" .
game.exe
```

Each SDL2 project folder contains a `build.bat` with these commands already filled in.

### 4. SDL2 project (CMake)

The `CMakeLists.txt` in SDL2 projects uses `find_package`-style paths pointing to the shared SDL2 location. Run:
```bat
cmake -B build -G "MinGW Makefiles"
cmake --build build
copy "C:\Users\zen\Documents\GitHub\C-PROGRAM\SDL2\SDL2-2.32.8\x86_64-w64-mingw32\bin\SDL2.dll" build\
build\game.exe
```

---

## C++ Standard Support (g++ 16.1.0)

- **C++17**: Full support — use `-std=c++17`
- **C++20**: Full support — use `-std=c++20` (concepts, ranges, std::span, coroutines)
- **C++23**: Partial — some features available with `-std=c++23`

---

## Quick Start

### Run Day 1 of BEGINNER
```bat
cd BEGINNER\day01
build.bat
```

### Run a SYSTEMS_PERFORMANCE program
```bat
cd ADVANCED\SYSTEMS_PERFORMANCE
g++ -std=c++17 -O2 -o custom_memory_pool.exe custom_memory_pool.cpp
custom_memory_pool.exe
```

### Run a GAME_DEV project
```bat
cd ADVANCED\GAME_DEV\space_shooter_sdl2
build.bat
space_shooter.exe
```
