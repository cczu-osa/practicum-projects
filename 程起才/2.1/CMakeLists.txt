cmake_minimum_required(VERSION 3.5)
project(2_1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c LinkedList.h LinkedList.c management.h management.c student.h)
add_executable(2_1 ${SOURCE_FILES})
