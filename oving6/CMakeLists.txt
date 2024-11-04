cmake_minimum_required(VERSION 3.5)

project(Oving6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra")

add_executable(compress compress.cpp file.cpp) 
add_executable(decompress decompress.cpp file.cpp)
