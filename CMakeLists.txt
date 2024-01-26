cmake_minimum_required(VERSION 3.15)

option(REFLECTCPP_BUILD_SHARED "Build shared library" OFF)
option(REFLECTCPP_BSON "Enable BSON support" OFF)
option(REFLECTCPP_CBOR "Enable CBOR support" OFF)
option(REFLECTCPP_FLEXBUFFERS "Enable flexbuffers support" OFF)
option(REFLECTCPP_XML "Enable XML support" OFF)
option(REFLECTCPP_YAML "Enable YAML support" OFF)

option(REFLECTCPP_BUILD_TESTS "Build tests" OFF)

# enable vcpkg if require features other than JSON
if (REFLECTCPP_BSON OR REFLECTCPP_CBOR OR REFLECTCPP_FLEXBUFFERS OR REFLECTCPP_XML OR REFLECTCPP_YAML)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
endif ()

project(reflectcpp)

set(CMAKE_CXX_STANDARD 20)

if (REFLECTCPP_BUILD_SHARED)
    add_library(reflectcpp SHARED src/yyjson.c)
else ()
    add_library(reflectcpp STATIC src/yyjson.c)
endif ()

target_include_directories(reflectcpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (REFLECTCPP_BSON)
    find_package(bson-1.0 CONFIG REQUIRED)
    target_link_libraries(reflectcpp PRIVATE $<IF:$<TARGET_EXISTS:mongo::bson_static>,mongo::bson_static,mongo::bson_shared>) 
endif ()

if (REFLECTCPP_CBOR)
    if (MSVC)
        target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/tinycbor${CMAKE_STATIC_LIBRARY_SUFFIX}")
    else ()
        target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libtinycbor${CMAKE_STATIC_LIBRARY_SUFFIX}")
    endif ()
endif ()

if (REFLECTCPP_FLEXBUFFERS)
    find_package(flatbuffers CONFIG REQUIRED)
    target_link_libraries(reflectcpp INTERFACE flatbuffers::flatbuffers)
endif ()

if (REFLECTCPP_XML)
    find_package(pugixml CONFIG REQUIRED)
    target_link_libraries(reflectcpp INTERFACE pugixml::pugixml)
endif ()

if (REFLECTCPP_YAML)
    find_package(yaml-cpp CONFIG REQUIRED)
    target_link_libraries(reflectcpp INTERFACE yaml-cpp::yaml-cpp)
endif ()

target_compile_options(reflectcpp PRIVATE -Wall)

if (REFLECTCPP_BUILD_TESTS)
    add_subdirectory(tests)
endif ()
