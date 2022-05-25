cmake_minimum_required(VERSION 3.22)
project(lesson1)

set(CMAKE_CXX_STANDARD 17)

add_executable(server main_server.cpp UDP_Server.cpp)
add_executable(client main_client.cpp)
