cmake_minimum_required(VERSION 3.8)
project(eratosthenes CXX)
add_executable(a.out main.cpp eratosthenes.cpp)
set(CMAKE_CXX_FLAGS "-std=c++17 -O2 -mtune=native -march=native -flto -pthread -Wall")
