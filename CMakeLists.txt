cmake_minimum_required(VERSION 3.9)
project(dod)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-O3 -g -pedantic-errors -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual \
        -Wstrict-prototypes -Wmissing-prototypes")

add_executable(test_vec test_vec.c dodret.h dodbytes.h dodvec.h)
add_executable(test_dict test_dict.c dodret.h dodbytes.h dodvec.h doddict.h)
add_executable(test_bs test_bs.c dodbs.h)
