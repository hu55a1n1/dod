cmake_minimum_required(VERSION 3.10)
project(cbytes)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -g")

add_executable(test_cvec test_cvec.c cvec.h)

add_executable(testcpp testcpp.cpp cvec.h cvector.h)