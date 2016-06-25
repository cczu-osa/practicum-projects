cmake_minimum_required(VERSION 3.5)
project(6_scv)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(simplecv)

set(SOURCE_FILES main.cpp)
add_executable(6_scv ${SOURCE_FILES})
target_link_libraries (6_scv LINK_PUBLIC SimpleCV)
