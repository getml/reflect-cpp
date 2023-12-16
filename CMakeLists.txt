cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(reflectcpp)

option(REFLECTCPP_ENABLE_ALL "Enable all features" OFF)
option(REFLECTCPP_JSON "Enable JSON support" OFF)
option(REFLECTCPP_FLATBUFFERS "Enable FlatBuffers support" OFF)

option(REFLECTCPP_BUILD_TESTS "Build tests" OFF)

if (REFLECTCPP_BUILD_TESTS)
    set(REFLECTCPP_ENABLE_ALL ON)
endif ()

if (REFLECTCPP_ENABLE_ALL)
    set(REFLECTCPP_JSON ON)
    set(REFLECTCPP_FLATBUFFERS ON)
endif ()

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

if (REFLECTCPP_BUILD_TESTS)
    add_subdirectory(tests)
endif ()