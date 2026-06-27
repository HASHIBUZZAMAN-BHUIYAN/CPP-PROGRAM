@echo off
echo Building ADVANCED ROBOTICS_CV programs...
echo.

echo [1/4] eigen_linear_algebra_robotics.cpp...
g++ -std=c++17 -Wall -O2 -o eigen_linear_algebra_robotics.exe eigen_linear_algebra_robotics.cpp
if %errorlevel% neq 0 (echo FAIL) else (echo OK)

echo [2/4] pid_controller_cpp.cpp...
g++ -std=c++17 -Wall -O2 -o pid_controller_cpp.exe pid_controller_cpp.cpp
if %errorlevel% neq 0 (echo FAIL) else (echo OK)

echo [3/4] opencv_basic_vision_cpp.cpp...
g++ -std=c++17 -Wall -O2 -o opencv_basic_vision_cpp.exe opencv_basic_vision_cpp.cpp
if %errorlevel% neq 0 (echo FAIL) else (echo OK)

echo [4/4] ros2_style_node_pattern_cpp.cpp...
g++ -std=c++17 -Wall -O2 -o ros2_style_node_pattern_cpp.exe ros2_style_node_pattern_cpp.cpp
if %errorlevel% neq 0 (echo FAIL) else (echo OK)

echo.
echo All built. Run each .exe to see output.
