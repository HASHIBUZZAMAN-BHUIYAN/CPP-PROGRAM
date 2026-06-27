cmake_minimum_required(VERSION 3.16)
project(cmake_multi_module VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- mathutils static library ----
add_library(mathutils STATIC
    src/stats.cpp
    src/geometry.cpp
)
target_include_directories(mathutils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_options(mathutils PRIVATE -Wall -O2)

# ---- Main application ----
add_executable(math_app src/main.cpp)
target_link_libraries(math_app PRIVATE mathutils)
target_compile_options(math_app PRIVATE -Wall -O2)

# ---- Tests ----
add_executable(math_tests tests/test_mathutils.cpp)
target_link_libraries(math_tests PRIVATE mathutils)
target_compile_options(math_tests PRIVATE -Wall -O2)

# ---- Install message ----
message(STATUS "Targets: math_app, math_tests")
message(STATUS "Build:   cmake -B build -G \"MinGW Makefiles\" && cmake --build build")
