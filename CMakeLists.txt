cmake_minimum_required(VERSION 3.4)

project(reflectcpp)

include_directories(
    "include"
)

add_library(reflectcpp SHARED "src/yyjson.c")
add_library(reflectcpp_static STATIC "src/yyjson.c")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")
