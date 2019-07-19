cmake_minimum_required(VERSION 3.10)
project(ucutils)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-O3 -g -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual \
        -Wstrict-prototypes -Wmissing-prototypes")

add_executable(test_ucvec test_ucvec.c ucret.h ucbytes.h ucvec.h)
add_executable(test_ucdict test_ucdict.c ucret.h ucbytes.h ucvec.h ucdict.h)
