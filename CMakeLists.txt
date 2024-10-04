cmake_minimum_required(VERSION 3.27)
project(working)

set(CMAKE_CXX_STANDARD 14)

add_executable(traceroute
        main.cpp
        tracing.h
        tracing.cpp)
