cmake_minimum_required(VERSION 3.5)

add_library(SimpleCV types.h core.h core.cpp io.cpp io.h scv.h matrix.cpp matrix.h)
target_include_directories(SimpleCV PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
