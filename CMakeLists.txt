cmake_minimum_required(VERSION 3.20)
project(app LANGUAGES CXX)

# Use C++20 for modern features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate the system-installed SDL3
find_package(SDL3 REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/renderer.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

# Link against the SDL3 shared library target
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
