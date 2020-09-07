cmake_minimum_required(VERSION 3.17)
project(PyPKG)

set(CMAKE_CXX_STANDARD 17)

add_executable (PyPKG WIN32 main.cpp console.cpp)