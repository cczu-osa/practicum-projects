cmake_minimum_required(VERSION 3.5)
project(5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)

file(GLOB SOURCE_FILES "*.h" "*.c" "LogUtils/*.h" "LogUtils/*.c")
add_executable(5 ${SOURCE_FILES})
target_link_libraries(5 ${OpenCV_LIBS})
