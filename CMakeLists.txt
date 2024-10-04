cmake_minimum_required(VERSION 3.27)
project(working)

set(CMAKE_CXX_STANDARD 14)

add_executable(hvtraceroute
        main.cpp
        tracing.h
        tracing.cpp)

install(TARGETS traceroute DESTINATION /usr/local/bin)
