cmake_minimum_required(VERSION 3.4)

project(reflect-cpp-tests)

include_directories(
    "../include"
)

add_executable(
    reflect-cpp-tests
    "tests.cpp"
    "../src/yyjson.c"
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wno-sign-compare -Wno-missing-braces -Wno-psabi -pthread -fno-strict-aliasing -fwrapv -O2")
