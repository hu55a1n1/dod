cmake_minimum_required(VERSION 3.12)
project(cbytes)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

add_executable(test_cvec test_cvec.c cvec.h)

add_executable(testcpp testcpp.cpp cvec.h)