cmake_minimum_required(VERSION 3.5)
project(1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB SOURCE_FILES "*.h" "*.c" "DataStructure/*.h" "DataStructure/*.c" "LogUtils/*.h" "LogUtils/*.c")
add_executable(1 ${SOURCE_FILES})
