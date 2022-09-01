cmake_minimum_required(VERSION 3.20)
project(proj1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(proj1
        graph.cpp
        graph.h
        driver.cpp
        data.txt
        testdata.txt
        )