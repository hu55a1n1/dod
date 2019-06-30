cmake_minimum_required(VERSION 3.10)
project(ucutils)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -Wextra -O3 -g")

add_executable(test_ucvec test_ucvec.c ucvec.h ucbytes.h)
