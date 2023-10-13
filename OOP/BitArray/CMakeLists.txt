cmake_minimum_required(VERSION 3.26)
project(OOP)

set(CMAKE_CXX_STANDARD 23)

add_executable(OOP test.cpp
        mybitarray.cpp
        mybitarray.h)
# Add the googletest subdirectory
add_subdirectory(googletest)

# Link the gtest and gtest_main targets against your target
target_link_libraries(OOP gtest gtest_main)

#add_subdirectory(src)
#add_subdirectory(test)