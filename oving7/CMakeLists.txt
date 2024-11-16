cmake_minimum_required(VERSION 3.5)

project(Oving7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra")

add_executable(dijkstras dijkstras_main.cpp map.cpp dijkstras.cpp util.cpp)
add_executable(alt alt.cpp map.cpp dijkstras.cpp util.cpp)
