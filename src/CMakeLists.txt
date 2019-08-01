cmake_minimum_required(VERSION 3.9)
project(TCP_Server)

set(CMAKE_CXX_STANDARD 11)

add_executable(TCP_Server main.cpp TCP_Server.cpp TCP_Server.h)
