cmake_minimum_required(VERSION 3.12)

project(
        ArgParser
        VERSION 1.0
        DESCRIPTION "C++ CLI Argument Parser"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

add_subdirectory(bin)
add_subdirectory(lib)
