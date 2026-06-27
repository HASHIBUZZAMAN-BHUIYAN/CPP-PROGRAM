@echo off
echo Building Day 01 programs...
g++ -std=c++17 -Wall -o lesson.exe lesson.cpp
g++ -std=c++17 -Wall -o exercises.exe exercises.cpp
g++ -std=c++17 -Wall -o solutions.exe solutions.cpp
g++ -std=c++17 -Wall -o mini_project.exe mini_project.cpp
echo Done. Run: lesson.exe  /  exercises.exe  /  solutions.exe  /  mini_project.exe
