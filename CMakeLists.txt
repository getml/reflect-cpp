cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(reflectcpp)

option(REFLECTCPP_JSON "Enable JSON support" OFF)
option(REFLECTCPP_FLATBUFFERS "Enable FlatBuffers support" OFF)

add_library(reflectcpp INTERFACE)

target_include_directories(reflectcpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (REFLECTCPP_JSON)
    find_package(yyjson CONFIG REQUIRED)
    target_link_libraries(reflectcpp INTERFACE yyjson::yyjson)
endif ()

if (REFLECTCPP_FLATBUFFERS)
    find_package(flatbuffers CONFIG REQUIRED)
    target_link_libraries(reflectcpp INTERFACE flatbuffers::flatbuffers)
endif ()


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")


option(REFLECTCPP_BUILD_TESTS "Build tests" OFF)

if (REFLECTCPP_BUILD_TESTS)
    add_subdirectory(tests)
endif ()