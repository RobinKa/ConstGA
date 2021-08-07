cmake_minimum_required (VERSION 3.8)

project(ConstGA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("constga")
add_subdirectory("cli")
add_subdirectory("tests")