@echo off
echo Building Day 07 (tests)...
g++ -std=c++17 -Wall -o tests.exe lesson.cpp
echo Done. Run: tests.exe
echo.
echo Note: If doctest.h is present in this folder, real doctest framework is used.
echo       Otherwise a manual fallback test runner runs instead.
echo.
echo To get doctest:
echo   1. Go to: github.com/doctest/doctest/releases
echo   2. Download doctest.h from the latest release
echo   3. Place doctest.h in this folder
echo   4. Rebuild with: g++ -std=c++17 -Wall -o tests.exe lesson.cpp
