cmake_minimum_required(VERSION 3.10)
project(BlackjackSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Aggressive optimizations for Monte Carlo execution speed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")

# Tell CMake where to find the header files
include_directories(include)

# Define the executable and its source files
add_executable(BlackjackSolver src/main.cpp src/Shoe.cpp src/Simulator.cpp)