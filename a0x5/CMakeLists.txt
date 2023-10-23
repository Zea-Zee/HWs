cmake_minimum_required(VERSION 3.26)
project(a0x5)

set(CMAKE_CXX_STANDARD 23)

add_executable(a0x5 main.cpp
        module1.ixx
        module2.ixx)
