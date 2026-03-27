cmake_minimum_required(VERSION 3.10)
project(QuantPokerSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Aggressive optimizations for maximum Monte Carlo throughput
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")

include_directories(include)

add_executable(PokerSolver src/main.cpp src/Evaluator.cpp)