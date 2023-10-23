cmake_minimum_required(VERSION 3.26)
project(BitArray)

set(CMAKE_CXX_STANDARD 23)

add_executable(OOP mybitarray.cpp
        test.cpp
        mybitarray.h)

add_subdirectory(googletest)
target_link_libraries(OOP gtest gtest_main)