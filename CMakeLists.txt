cmake_minimum_required(VERSION 3.3)
project(snek)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS 0)

add_executable(snek example.cpp)
add_library(snektest SHARED testlib.cpp)
