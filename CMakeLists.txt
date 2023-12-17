cmake_minimum_required(VERSION 3.15)

option(REFLECTCPP_BUILD_SHARED "Build shared library" OFF)
option(REFLECTCPP_FLEXBUFFERS "Enable flexbuffers support" OFF)

option(REFLECTCPP_BUILD_TESTS "Build tests" OFF)


# enable vcpkg if require features other than JSON
if (REFLECTCPP_FLEXBUFFERS)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
endif ()

project(reflectcpp)

if (REFLECTCPP_BUILD_SHARED)
    add_library(reflectcpp SHARED src/yyjson.c)
else ()
    add_library(reflectcpp STATIC src/yyjson.c)
endif ()
target_include_directories(reflectcpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


if (REFLECTCPP_FLEXBUFFERS)
    find_package(flatbuffers CONFIG REQUIRED)
    target_link_libraries(reflectcpp INTERFACE flatbuffers::flatbuffers)
endif ()

target_compile_options(reflectcpp PRIVATE -Wall)

if (REFLECTCPP_BUILD_TESTS)
    add_subdirectory(tests)
endif ()