cmake_minimum_required(VERSION 3.9)
project(databases)

set(CMAKE_CXX_STANDARD 11)

add_executable(databases main.cpp Table.cpp headers/Table.h FuncDepen.cpp headers/FuncDepen.h)

