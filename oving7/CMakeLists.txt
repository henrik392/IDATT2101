cmake_minimum_required(VERSION 3.5)

project(Oving7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra")

add_executable(dijkstra dijkstra_main.cpp map.cpp dijkstra.cpp util.cpp)
add_executable(alt alt.cpp map.cpp dijkstra.cpp util.cpp)
