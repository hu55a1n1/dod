cmake_minimum_required(VERSION 3.10)
project(cbytes)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -g")

add_executable(test_cvec test_cvec.c cvec.h cb.h)
add_executable(testcpp testcpp.cpp cvec.h cvector.h)

if( supported )
    message(STATUS "IPO / LTO enabled")
    set_property(TARGET test_cvec PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET testcpp PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO / LTO not supported: <${error}>")
endif()