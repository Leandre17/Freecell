cmake_minimum_required(VERSION 3.10)
project(Freecell VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(freecell main.cpp)
