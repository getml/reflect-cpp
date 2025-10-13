cmake_minimum_required(VERSION 3.23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(REFLECTCPP_BUILD_SHARED "Build shared library" ${BUILD_SHARED_LIBS})
option(REFLECTCPP_INSTALL "Install reflect cpp" OFF)

option(REFLECTCPP_ALL_FORMATS "Enable all supported formats" OFF)
option(REFLECTCPP_JSON "Enable JSON support" ON) # enabled by default
option(REFLECTCPP_AVRO "Enable AVRO support" ${REFLECTCPP_ALL_FORMATS})
option(REFLECTCPP_BSON "Enable BSON support" ${REFLECTCPP_ALL_FORMATS})
option(REFLECTCPP_CAPNPROTO "Enable Cap’n Proto support" ${REFLECTCPP_ALL_FORMATS})
option(REFLECTCPP_CBOR "Enable CBOR support" ${REFLECTCPP_ALL_FORMATS})
option(REFLECTCPP_CSV "Enable CSV support" ${REFLECTCPP_ALL_FORMATS})
option(REFLECTCPP_FLEXBUFFERS "Enable flexbuffers support" ${REFLECTCPP_ALL_FORMATS})
option(REFLECTCPP_MSGPACK "Enable msgpack support" ${REFLECTCPP_ALL_FORMATS})
option(REFLECTCPP_PARQUET "Enable parquet support" ${REFLECTCPP_ALL_FORMATS})
option(REFLECTCPP_XML "Enable XML support" ${REFLECTCPP_ALL_FORMATS})
option(REFLECTCPP_TOML "Enable TOML support" ${REFLECTCPP_ALL_FORMATS})
option(REFLECTCPP_UBJSON "Enable UBJSON support" ${REFLECTCPP_ALL_FORMATS})
option(REFLECTCPP_YAML "Enable YAML support" ${REFLECTCPP_ALL_FORMATS})

option(REFLECTCPP_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(REFLECTCPP_BUILD_TESTS "Build tests" OFF)

option(REFLECTCPP_USE_BUNDLED_DEPENDENCIES "Use the bundled dependencies" ON)

option(REFLECTCPP_USE_STD_EXPECTED "Use std::expected instead of the built-in Result type (requires C++-23)" OFF)

if(REFLECTCPP_USE_STD_EXPECTED)
    if(NOT DEFINED CMAKE_CXX_STANDARD)
        set(CMAKE_CXX_STANDARD 23)
    endif()
else()
    if(NOT DEFINED CMAKE_CXX_STANDARD)
        set(CMAKE_CXX_STANDARD 20)
    endif()
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(REFLECTCPP_USE_VCPKG_DEFAULT OFF)

if(REFLECTCPP_BUILD_BENCHMARKS)
    set(REFLECTCPP_JSON ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_AVRO ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_BSON ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_CAPNPROTO ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_CBOR ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_FLEXBUFFERS ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_MSGPACK ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_XML ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_TOML ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_UBJSON ON CACHE BOOL "" FORCE)
    set(REFLECTCPP_YAML ON CACHE BOOL "" FORCE)
endif()

if (REFLECTCPP_BUILD_TESTS OR REFLECTCPP_BUILD_BENCHMARKS OR
    (REFLECTCPP_JSON AND NOT REFLECTCPP_USE_BUNDLED_DEPENDENCIES) OR REFLECTCPP_AVRO OR
    REFLECTCPP_BSON OR REFLECTCPP_CAPNPROTO OR REFLECTCPP_CBOR OR REFLECTCPP_CSV OR
    REFLECTCPP_FLEXBUFFERS OR REFLECTCPP_MSGPACK OR REFLECTCPP_PARQUET OR REFLECTCPP_XML OR 
    REFLECTCPP_TOML OR REFLECTCPP_UBJSON OR REFLECTCPP_YAML)
    # enable vcpkg per default if features other than JSON are required
    set(REFLECTCPP_USE_VCPKG_DEFAULT ON)
endif()

if(REFLECTCPP_JSON OR REFLECTCPP_AVRO OR REFLECTCPP_CBOR OR REFLECTCPP_UBJSON)
    set(_REFLECTCPP_NEEDS_JSON_IMPL ON)
else()
    set(_REFLECTCPP_NEEDS_JSON_IMPL OFF)
endif()

option(REFLECTCPP_USE_VCPKG "Use VCPKG to download and build dependencies" ${REFLECTCPP_USE_VCPKG_DEFAULT})

if (REFLECTCPP_USE_VCPKG)
    if (REFLECTCPP_AVRO)
        list(APPEND VCPKG_MANIFEST_FEATURES "avro")
    endif()
    
    if (REFLECTCPP_BSON)
        list(APPEND VCPKG_MANIFEST_FEATURES "bson")
    endif()
    
    if (REFLECTCPP_BUILD_BENCHMARKS)
        list(APPEND VCPKG_MANIFEST_FEATURES "benchmarks")
    endif()

    if (REFLECTCPP_BUILD_TESTS)
        list(APPEND VCPKG_MANIFEST_FEATURES "tests")
    endif()
    
    if (REFLECTCPP_CAPNPROTO)
        list(APPEND VCPKG_MANIFEST_FEATURES "capnproto")
    endif()
    
    if (REFLECTCPP_CBOR)
        list(APPEND VCPKG_MANIFEST_FEATURES "cbor")
    endif()
    
    if (REFLECTCPP_CSV)
        list(APPEND VCPKG_MANIFEST_FEATURES "csv")
    endif()
    
    if (NOT REFLECTCPP_USE_BUNDLED_DEPENDENCIES)
        list(APPEND VCPKG_MANIFEST_FEATURES "ctre")
    endif()
    
    if (REFLECTCPP_FLEXBUFFERS)
        list(APPEND VCPKG_MANIFEST_FEATURES "flexbuffers")
    endif()
    
    if (_REFLECTCPP_NEEDS_JSON_IMPL AND NOT REFLECTCPP_USE_BUNDLED_DEPENDENCIES)
        list(APPEND VCPKG_MANIFEST_FEATURES "json")
    endif()
    
    if (REFLECTCPP_MSGPACK)
        list(APPEND VCPKG_MANIFEST_FEATURES "msgpack")
    endif()
     
    if (REFLECTCPP_PARQUET)
        list(APPEND VCPKG_MANIFEST_FEATURES "parquet")
    endif()
    
    if (REFLECTCPP_TOML)
        list(APPEND VCPKG_MANIFEST_FEATURES "toml")
    endif()
    
    if (REFLECTCPP_UBJSON)
        list(APPEND VCPKG_MANIFEST_FEATURES "ubjson")
    endif()

    if (REFLECTCPP_XML)
        list(APPEND VCPKG_MANIFEST_FEATURES "xml")
    endif()
    
    if (REFLECTCPP_YAML)
        list(APPEND VCPKG_MANIFEST_FEATURES "yaml")
    endif()
    
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
endif ()

project(reflectcpp VERSION 0.21.0 LANGUAGES CXX)

if (PROJECT_IS_TOP_LEVEL)
    set(REFLECTCPP_INSTALL ON)
endif()

if (REFLECTCPP_BUILD_SHARED)
    add_library(reflectcpp SHARED)
    set_target_properties(reflectcpp PROPERTIES SOVERSION ${PROJECT_VERSION})
else()
    add_library(reflectcpp STATIC)
endif()

add_library(reflectcpp::reflectcpp ALIAS reflectcpp)

if (MSVC)
    target_compile_options(reflectcpp PRIVATE 
        $<$<CONFIG:Debug>:
            -Wall
        >
        $<$<CONFIG:Release>:
            -DNDEBUG
        >
    )
else()
    target_compile_options(reflectcpp PRIVATE
        $<$<CONFIG:Debug>:
            -Wall -Wextra -Wpedantic -Wshadow -Wconversion
            #-Wnull-dereference -Wold-style-cast
            #-g3 -fno-omit-frame-pointer
            #-fsanitize=address,undefined
            #-fno-sanitize-recover=undefined
        >
        $<$<CONFIG:Release>:
            -DNDEBUG
        >
    )
endif()

if(REFLECTCPP_USE_STD_EXPECTED)
    target_compile_definitions(reflectcpp PUBLIC REFLECTCPP_USE_STD_EXPECTED)
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

if (_REFLECTCPP_NEEDS_JSON_IMPL)
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

if (REFLECTCPP_AVRO)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_avro.cpp
    )
    find_package(jansson CONFIG REQUIRED)
    if (REFLECTCPP_USE_VCPKG)
        target_include_directories(reflectcpp SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
        if (MSVC)
            target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/avro${CMAKE_STATIC_LIBRARY_SUFFIX}")
        else ()
            target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libavro${CMAKE_STATIC_LIBRARY_SUFFIX}")
        endif ()
    else ()
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(avro REQUIRED IMPORTED_TARGET avro-c)
        target_link_libraries(reflectcpp PUBLIC PkgConfig::avro)
    endif ()
    target_link_libraries(reflectcpp PRIVATE jansson::jansson)
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

if (REFLECTCPP_CAPNPROTO)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_capnproto.cpp
    )
    if (NOT TARGET CapnProto)
        find_package(CapnProto CONFIG REQUIRED)
    endif ()
    set(CAPNPC_LIB CapnProto::capnpc)
    if (NOT TARGET CapnProto::capnpc)
      # in some old versions, there is no alias target CapnProto::capnpc, fallback to capnc
      set(CAPNPC_LIB capnpc)
    endif()
    target_link_libraries(reflectcpp PUBLIC CapnProto::kj CapnProto::capnp ${CAPNPC_LIB} CapnProto::kj-gzip)
endif ()

if (REFLECTCPP_CBOR)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_cbor.cpp
    )
    if (NOT TARGET jsoncons)
      find_package(jsoncons CONFIG REQUIRED)
    endif()
    include_directories(PUBLIC ${jsoncons_INCLUDE_DIRS})
endif ()

if (REFLECTCPP_CSV)
    if (NOT TARGET Arrow)
        find_package(Arrow CONFIG REQUIRED)
    endif()
    if (REFLECTCPP_USE_VCPKG)
        target_link_libraries(reflectcpp PUBLIC "$<IF:$<BOOL:${ARROW_BUILD_STATIC}>,Arrow::arrow_static,Arrow::arrow_shared>")
    else()
        target_link_libraries(reflectcpp PUBLIC "arrow::arrow")
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
    if (NOT TARGET msgpack-c)
        find_package(msgpack-c CONFIG REQUIRED NAMES msgpack msgpack-c)
    endif()
    target_link_libraries(reflectcpp PUBLIC msgpack-c)
endif ()

if (REFLECTCPP_PARQUET)
    if (NOT TARGET Arrow)
        find_package(Arrow CONFIG REQUIRED)
    endif()
    if (REFLECTCPP_USE_VCPKG)
        if (NOT TARGET Parquet)
            find_package(Parquet CONFIG REQUIRED)
        endif()
        target_link_libraries(reflectcpp PUBLIC "$<IF:$<BOOL:${ARROW_BUILD_STATIC}>,Arrow::arrow_static,Arrow::arrow_shared>")
        target_link_libraries(reflectcpp PUBLIC "$<IF:$<BOOL:${ARROW_BUILD_STATIC}>,Parquet::parquet_static,Parquet::parquet_shared>")
    else()
        target_link_libraries(reflectcpp PUBLIC "arrow::arrow")
    endif()
endif ()

if (REFLECTCPP_TOML)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_toml.cpp
    )
    if (NOT TARGET tomlplusplus)
      find_package(tomlplusplus)
    endif()
    target_link_libraries(reflectcpp PUBLIC tomlplusplus::tomlplusplus)
endif()

if (REFLECTCPP_UBJSON)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_ubjson.cpp
    )
    if (NOT TARGET jsoncons)
      find_package(jsoncons CONFIG REQUIRED)
    endif()
    include_directories(PUBLIC ${jsoncons_INCLUDE_DIRS})
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

if (REFLECTCPP_INSTALL)
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


# CPack configuration

# Set general package information
set(CPACK_PACKAGE_NAME "reflectcpp")
set(CPACK_PACKAGE_VENDOR "Reflect C++ Team")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Reflect C++ library")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "maintainer@reflectcpp.org")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# Configure generators
set(CPACK_GENERATOR "TGZ;RPM;DEB")

# DEB specific configuration
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
# Fix maintainer information - ensure both are set consistently
set(CPACK_PACKAGE_CONTACT "maintainer@reflectcpp.org")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "maintainer@reflectcpp.org")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")

# RPM specific configuration
set(CPACK_RPM_PACKAGE_REQUIRES "")

include(CPack)

