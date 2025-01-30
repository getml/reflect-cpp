cmake_minimum_required(VERSION 3.23)

option(REFLECTCPP_BUILD_SHARED "Build shared library" ${BUILD_SHARED_LIBS})

option(REFLECTCPP_JSON "Enable JSON support" ON) # enabled by default
option(REFLECTCPP_BSON "Enable BSON support" OFF)
option(REFLECTCPP_CBOR "Enable CBOR support" OFF)
option(REFLECTCPP_FLEXBUFFERS "Enable flexbuffers support" OFF)
option(REFLECTCPP_MSGPACK "Enable msgpack support" OFF)
option(REFLECTCPP_XML "Enable XML support" OFF)
option(REFLECTCPP_TOML "Enable TOML support" OFF)
option(REFLECTCPP_UBJSON "Enable UBJSON support" OFF)
option(REFLECTCPP_YAML "Enable YAML support" OFF)

option(REFLECTCPP_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(REFLECTCPP_BUILD_TESTS "Build tests" OFF)

option(REFLECTCPP_USE_BUNDLED_DEPENDENCIES "Use the bundled dependencies" ON)

set(REFLECTCPP_USE_VCPKG_DEFAULT OFF)
if(REFLECTCPP_BUILD_BENCHMARKS)
    set(REFLECTCPP_JSON ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_BSON ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_CBOR ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_FLEXBUFFERS ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_MSGPACK ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_XML ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_TOML ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_UBJSON ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_YAML ON CACHE BOOL "" FORCE)
endif()
if (REFLECTCPP_BUILD_TESTS OR REFLECTCPP_BUILD_BENCHMARKS OR
    (REFLECTCPP_JSON AND NOT REFLECTCPP_USE_BUNDLED_DEPENDENCIES) OR
    REFLECTCPP_BSON OR REFLECTCPP_CBOR OR REFLECTCPP_FLEXBUFFERS OR REFLECTCPP_MSGPACK OR REFLECTCPP_XML OR REFLECTCPP_TOML OR REFLECTCPP_UBJSON OR REFLECTCPP_YAML)
    # enable vcpkg per default if require features other than JSON
    set(REFLECTCPP_USE_VCPKG_DEFAULT ON)
endif()

option(REFLECTCPP_USE_VCPKG "Use VCPKG to download and build dependencies" ${REFLECTCPP_USE_VCPKG_DEFAULT})

if (REFLECTCPP_USE_VCPKG)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
endif ()

project(reflectcpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if (REFLECTCPP_BUILD_SHARED)
    add_library(reflectcpp SHARED)
else()
    add_library(reflectcpp STATIC)
endif()

if (MSVC)
    target_compile_options(reflectcpp PRIVATE $<$<CONFIG:Debug>:-Wall>)
else()
    target_compile_options(reflectcpp PRIVATE $<$<CONFIG:Debug>:-Wall -Wextra>)
endif()

target_compile_features(reflectcpp PUBLIC cxx_std_20)

set(REFLECT_CPP_SOURCES
    src/reflectcpp.cpp
)

target_include_directories(
    reflectcpp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

if (REFLECTCPP_USE_BUNDLED_DEPENDENCIES)
    target_include_directories(
        reflectcpp PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/rfl/thirdparty>)
else ()
    find_package(ctre CONFIG REQUIRED)
    target_link_libraries(reflectcpp INTERFACE ctre::ctre)
endif ()

if (REFLECTCPP_JSON)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_json.cpp
    )
    if (REFLECTCPP_USE_BUNDLED_DEPENDENCIES)
        list(APPEND REFLECT_CPP_SOURCES
            src/yyjson.c
        )
        set_source_files_properties(src/yyjson.c PROPERTIES LANGUAGE CXX)

        target_include_directories(
            reflectcpp PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/rfl/thirdparty>)
    else ()
        find_package(yyjson CONFIG REQUIRED)
        target_link_libraries(reflectcpp INTERFACE yyjson::yyjson)
    endif ()
endif ()

if (REFLECTCPP_BSON)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_bson.cpp
    )
    find_package(bson-1.0 CONFIG REQUIRED)
    target_link_libraries(reflectcpp PRIVATE $<IF:$<TARGET_EXISTS:mongo::bson_static>,mongo::bson_static,mongo::bson_shared>)
endif ()

if (REFLECTCPP_CBOR)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_cbor.cpp
    )
    target_include_directories(reflectcpp SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
    target_include_directories(reflectcpp SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/tinycbor")
    if (MSVC)
        target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/tinycbor${CMAKE_STATIC_LIBRARY_SUFFIX}")
    else ()
        target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libtinycbor${CMAKE_STATIC_LIBRARY_SUFFIX}")
    endif ()
endif ()

if (REFLECTCPP_FLEXBUFFERS)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_flexbuf.cpp
    )
    find_package(flatbuffers CONFIG REQUIRED)
    target_link_libraries(reflectcpp INTERFACE flatbuffers::flatbuffers)
endif ()

if (REFLECTCPP_MSGPACK)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_msgpack.cpp
    )
    find_package(msgpack-c CONFIG REQUIRED)
    if (MSVC)
        target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/msgpack-c${CMAKE_STATIC_LIBRARY_SUFFIX}")
    else ()
        target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libmsgpack-c${CMAKE_STATIC_LIBRARY_SUFFIX}")
    endif ()
endif ()

if (REFLECTCPP_TOML)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_toml.cpp
    )
    if (MSVC)
        target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/tomlplusplus${CMAKE_STATIC_LIBRARY_SUFFIX}")
    else ()
        target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libtomlplusplus${CMAKE_STATIC_LIBRARY_SUFFIX}")
    endif ()
endif()

if (REFLECTCPP_UBJSON)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_ubjson.cpp
    )
    target_include_directories(reflectcpp SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
    find_package(jsoncons CONFIG REQUIRED)
endif ()

if (REFLECTCPP_XML)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_xml.cpp
    )
    find_package(pugixml CONFIG REQUIRED)
    target_link_libraries(reflectcpp INTERFACE pugixml::pugixml)
endif ()

if (REFLECTCPP_YAML)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_yaml.cpp
    )
    find_package(yaml-cpp CONFIG REQUIRED)
    target_link_libraries(reflectcpp INTERFACE yaml-cpp::yaml-cpp)
endif ()

set_target_properties(reflectcpp PROPERTIES LINKER_LANGUAGE CXX)
target_sources(reflectcpp PRIVATE ${REFLECT_CPP_SOURCES})
target_precompile_headers(reflectcpp PRIVATE [["rfl.hpp"]] <iostream> <string> <functional>)

if (REFLECTCPP_BUILD_TESTS)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    add_library(reflectcpp-cxx23)
    get_target_property(REFLECT_CPP_INCLUDE_DIRECTORIES reflectcpp INCLUDE_DIRECTORIES)
    target_sources(reflectcpp-cxx23 PRIVATE ${REFLECT_CPP_SOURCES})
    foreach (dir ${REFLECT_CPP_INCLUDE_DIRECTORIES})
        target_include_directories(reflectcpp-cxx23 PUBLIC ${dir})
    endforeach()
    set_target_properties(reflectcpp-cxx23 PROPERTIES CXX_STANDARD 23)
    set(REFLECT_CPP_GTEST_LIB reflectcpp GTest::gtest_main)
    set(REFLECT_CPP_GTEST_LIB_CXX23 reflectcpp-cxx23 GTest::gtest_main)
    add_subdirectory(tests)
endif ()

if (REFLECTCPP_BUILD_BENCHMARKS)
    find_package(benchmark CONFIG REQUIRED)
    find_package(simdjson CONFIG REQUIRED)
    target_link_libraries(reflectcpp INTERFACE benchmark::benchmark simdjson::simdjson)
    add_subdirectory(benchmarks)
endif ()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(reflectcpp-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/reflectcpp-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/reflectcpp
  )

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/reflectcpp-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/reflectcpp"
)

file(GLOB_RECURSE RFL_HEADERS RELATIVE ${CMAKE_CURRENT_LIST_DIR} "${CMAKE_CURRENT_LIST_DIR}/include/*" )

target_sources(reflectcpp
    PUBLIC
    FILE_SET reflectcpp_headers
    TYPE HEADERS
    BASE_DIRS $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    FILES ${RFL_HEADERS})

install(
    TARGETS reflectcpp
    EXPORT reflectcpp-exports
    FILE_SET reflectcpp_headers DESTINATION ${INCLUDE_INSTALL_DIR}
    )

install(
    EXPORT reflectcpp-exports
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/reflectcpp
    NAMESPACE reflectcpp::
)
