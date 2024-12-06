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
    if (NOT TARGET ctre::ctre)
        find_package(ctre CONFIG REQUIRED)
    endif ()
    target_link_libraries(reflectcpp PUBLIC ctre::ctre)
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
        if (NOT TARGET yyjson::yyjson)
            find_package(yyjson CONFIG REQUIRED)
        endif ()
        target_link_libraries(reflectcpp PUBLIC yyjson::yyjson)
    endif ()
endif ()

if (REFLECTCPP_BSON)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_bson.cpp
    )
    if (NOT TARGET mongo::bson_static AND NOT TARGET mongo::bson_shared)
        find_package(bson-1.0 CONFIG REQUIRED)
    endif ()
    target_link_libraries(reflectcpp PUBLIC $<IF:$<TARGET_EXISTS:mongo::bson_static>,mongo::bson_static,mongo::bson_shared>)
endif ()

if (REFLECTCPP_CBOR)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_cbor.cpp
    )
    if(REFLECTCPP_USE_VCPKG)
        target_include_directories(reflectcpp SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
        target_include_directories(reflectcpp SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/tinycbor")
        if (MSVC)
            target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/tinycbor${CMAKE_STATIC_LIBRARY_SUFFIX}")
        else ()
            target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libtinycbor${CMAKE_STATIC_LIBRARY_SUFFIX}")
        endif ()
    else()
        if (NOT TARGET tinycbor)
          find_package(tinycbor)
        endif()
        target_link_libraries(reflectcpp PUBLIC tinycbor::tinycbor)
    endif()
endif ()

if (REFLECTCPP_FLEXBUFFERS)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_flexbuf.cpp
    )
    if (NOT TARGET flatbuffers::flatbuffers)
        find_package(flatbuffers CONFIG REQUIRED)
    endif ()
    target_link_libraries(reflectcpp PUBLIC flatbuffers::flatbuffers)
endif ()

if (REFLECTCPP_MSGPACK)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_msgpack.cpp
    )
    if(REFLECTCPP_USE_VCPKG)
        if (NOT TARGET msgpack-c)
            find_package(msgpack-c CONFIG REQUIRED)
        endif()
    else()
        if (NOT TARGET msgpack)
          find_package(msgpack CONFIG REQUIRED)
        endif()
    endif()
    target_link_libraries(reflectcpp PUBLIC msgpack-c)
endif ()

if (REFLECTCPP_TOML)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_toml.cpp
    )
    if(REFLECTCPP_USE_VCPKG)
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(tomlplusplus REQUIRED IMPORTED_TARGET tomlplusplus)
        target_link_libraries(reflectcpp PUBLIC PkgConfig::tomlplusplus)
    else()
        if (NOT TARGET tomlplusplus)
          find_package(tomlplusplus)
        endif()
        target_link_libraries(reflectcpp PUBLIC tomlplusplus::tomlplusplus)
    endif()
endif()

if (REFLECTCPP_UBJSON)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_ubjson.cpp
    )
    if (NOT TARGET jsoncons)
      find_package(jsoncons CONFIG REQUIRED)
    endif()
endif ()

if (REFLECTCPP_XML)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_xml.cpp
    )
    if (NOT TARGET pugixml::pugixml)
        find_package(pugixml CONFIG REQUIRED)
    endif ()
    target_link_libraries(reflectcpp PUBLIC pugixml::pugixml)
endif ()

if (REFLECTCPP_YAML)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_yaml.cpp
    )
    if (NOT TARGET yaml-cpp::yaml-cpp)
        find_package(yaml-cpp CONFIG REQUIRED)
    endif ()
    target_link_libraries(reflectcpp PUBLIC yaml-cpp::yaml-cpp)
endif ()

set_target_properties(reflectcpp PROPERTIES LINKER_LANGUAGE CXX)
target_sources(reflectcpp PRIVATE ${REFLECT_CPP_SOURCES})
target_precompile_headers(reflectcpp PRIVATE [["rfl.hpp"]] <iostream> <string> <functional>)

if (REFLECTCPP_BUILD_TESTS)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    set(REFLECT_CPP_GTEST_LIB reflectcpp GTest::gtest_main)
    add_subdirectory(tests)
endif ()

if (REFLECTCPP_BUILD_BENCHMARKS)
    if (NOT TARGET benchmark::benchmark)
        find_package(benchmark CONFIG REQUIRED)
    endif ()
    if (NOT TARGET simdjson::simdjson)
        find_package(simdjson CONFIG REQUIRED)
    endif ()
    target_link_libraries(reflectcpp PUBLIC benchmark::benchmark simdjson::simdjson)
    add_subdirectory(benchmarks)
endif ()

if (PROJECT_IS_TOP_LEVEL)
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
endif ()
