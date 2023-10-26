cmake_minimum_required(VERSION 3.26.4)
project(modules)

set(CMAKE_CXX_STANDARD 20)

add_executable(modules)
target_sources(modules PRIVATE
        FILE_SET all_my_modules TYPE CXX_MODULES
        BASE_DIRS
        ${PROJECT_SOURCE_DIR}
        FILES
        mod1.cppm
)
target_sources(modules PRIVATE main.cpp)