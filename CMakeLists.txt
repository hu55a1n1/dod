cmake_minimum_required(VERSION 3.12)
project(cbytes)

set(CMAKE_C_STANDARD 99)

add_executable(test_cvec test_cvec.c cvec.h)

add_executable(testcpp testcpp.cpp cvec.h)