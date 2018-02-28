cmake_minimum_required(VERSION 3.8)
project(pirate)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -O0 -S -masm=intel")

set(SOURCE_FILES main.cpp)
add_executable(pirate ${SOURCE_FILES})