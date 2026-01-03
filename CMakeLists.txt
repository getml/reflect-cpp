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
option(REFLECTCPP_CHECK_HEADERS "Make sure that all headers are self-contained" OFF)

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

if (REFLECTCPP_BUILD_TESTS OR REFLECTCPP_BUILD_BENCHMARKS OR REFLECTCPP_CHECK_HEADERS OR
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
    if (REFLECTCPP_AVRO OR REFLECTCPP_CHECK_HEADERS)
        list(APPEND VCPKG_MANIFEST_FEATURES "avro")
    endif()
    
    if (REFLECTCPP_BSON OR REFLECTCPP_CHECK_HEADERS)
        list(APPEND VCPKG_MANIFEST_FEATURES "bson")
    endif()
    
    if (REFLECTCPP_BUILD_BENCHMARKS OR REFLECTCPP_CHECK_HEADERS)
        list(APPEND VCPKG_MANIFEST_FEATURES "benchmarks")
    endif()

    if (REFLECTCPP_BUILD_TESTS OR REFLECTCPP_CHECK_HEADERS)
        list(APPEND VCPKG_MANIFEST_FEATURES "tests")
    endif()
    
    if (REFLECTCPP_CAPNPROTO OR REFLECTCPP_CHECK_HEADERS)
        list(APPEND VCPKG_MANIFEST_FEATURES "capnproto")
    endif()
    
    if (REFLECTCPP_CBOR OR REFLECTCPP_CHECK_HEADERS)
        list(APPEND VCPKG_MANIFEST_FEATURES "cbor")
    endif()
    
    if (REFLECTCPP_CSV OR REFLECTCPP_CHECK_HEADERS)
        list(APPEND VCPKG_MANIFEST_FEATURES "csv")
    endif()
    
    if (NOT REFLECTCPP_USE_BUNDLED_DEPENDENCIES)
        list(APPEND VCPKG_MANIFEST_FEATURES "ctre")
    endif()
    
    if (REFLECTCPP_FLEXBUFFERS OR REFLECTCPP_CHECK_HEADERS)
        list(APPEND VCPKG_MANIFEST_FEATURES "flexbuffers")
    endif()
    
    if (_REFLECTCPP_NEEDS_JSON_IMPL AND NOT REFLECTCPP_USE_BUNDLED_DEPENDENCIES)
        list(APPEND VCPKG_MANIFEST_FEATURES "json")
    endif()
    
    if (REFLECTCPP_MSGPACK OR REFLECTCPP_CHECK_HEADERS)
        list(APPEND VCPKG_MANIFEST_FEATURES "msgpack")
    endif()
     
    if (REFLECTCPP_PARQUET OR REFLECTCPP_CHECK_HEADERS)
        list(APPEND VCPKG_MANIFEST_FEATURES "parquet")
    endif()
    
    if (REFLECTCPP_TOML OR REFLECTCPP_CHECK_HEADERS)
        list(APPEND VCPKG_MANIFEST_FEATURES "toml")
    endif()
    
    if (REFLECTCPP_UBJSON OR REFLECTCPP_CHECK_HEADERS)
        list(APPEND VCPKG_MANIFEST_FEATURES "ubjson")
    endif()

    if (REFLECTCPP_XML OR REFLECTCPP_CHECK_HEADERS)
        list(APPEND VCPKG_MANIFEST_FEATURES "xml")
    endif()
    
    if (REFLECTCPP_YAML OR REFLECTCPP_CHECK_HEADERS)
        list(APPEND VCPKG_MANIFEST_FEATURES "yaml")
    endif()
    
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
endif ()

project(reflectcpp VERSION 0.23.0 LANGUAGES CXX)

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

if(REFLECTCPP_BUILD_SHARED AND REFLECTCPP_USE_BUNDLED_DEPENDENCIES AND _REFLECTCPP_NEEDS_JSON_IMPL)
    target_compile_definitions(reflectcpp
        PRIVATE 
            YYJSON_EXPORTS 
        INTERFACE 
            YYJSON_IMPORTS
    )
endif()

if(REFLECTCPP_BUILD_SHARED)
    target_compile_definitions(reflectcpp PUBLIC RFL_BUILD_SHARED)
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

if (REFLECTCPP_AVRO OR REFLECTCPP_CHECK_HEADERS)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_avro.cpp
    )
    find_package(jansson CONFIG REQUIRED)
    if (REFLECTCPP_USE_VCPKG)
        target_include_directories(reflectcpp SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
        if (MSVC)
            set(_AVRO_STATIC_LIB "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/avro${CMAKE_STATIC_LIBRARY_SUFFIX}")
            if(REFLECTCPP_BUILD_SHARED)
                message(STATUS "With whole archive ${_AVRO_STATIC_LIB}")
                target_link_libraries(reflectcpp 
                    PUBLIC 
                        $<LINK_LIBRARY:WHOLE_ARCHIVE,${_AVRO_STATIC_LIB}> 
                        "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/snappy${CMAKE_STATIC_LIBRARY_SUFFIX}" 
                        "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/lzma${CMAKE_STATIC_LIBRARY_SUFFIX}" 
                        "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/jansson${CMAKE_STATIC_LIBRARY_SUFFIX}"
                        "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/zlib${CMAKE_STATIC_LIBRARY_SUFFIX}"
                )
            else()
                target_link_libraries(reflectcpp PUBLIC "${_AVRO_STATIC_LIB}")
            endif()
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

if (REFLECTCPP_BSON OR REFLECTCPP_CHECK_HEADERS)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_bson.cpp
    )

    # vcpkg doesn't yet have libbson>=2, where the package and target names have changed
    # https://github.com/mongodb/mongo-c-driver/releases/tag/2.0.0
    if(REFLECTCPP_USE_VCPKG)
        set(BSON_PACKAGE_NAME bson-1.0)
        set(bson_tgt_name_prefix "mongo::bson_")
    else()
        set(BSON_PACKAGE_NAME bson)
        set(bson_tgt_name_prefix "bson::")
    endif()

    if(REFLECTCPP_BUILD_SHARED)
        set(bson_tgt_linkage shared)
    else()
        set(bson_tgt_linkage static)
    endif()

    find_package(${BSON_PACKAGE_NAME} 1.25.1 REQUIRED)
    target_link_libraries(reflectcpp PUBLIC "${bson_tgt_name_prefix}${bson_tgt_linkage}")
endif ()

if (REFLECTCPP_CAPNPROTO OR REFLECTCPP_CHECK_HEADERS)
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

if (REFLECTCPP_CBOR OR REFLECTCPP_CHECK_HEADERS)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_cbor.cpp
    )
    if (NOT TARGET jsoncons)
      find_package(jsoncons CONFIG REQUIRED)
    endif()
    include_directories(PUBLIC ${jsoncons_INCLUDE_DIRS})
endif ()

if (REFLECTCPP_CSV OR REFLECTCPP_CHECK_HEADERS)
    if (NOT TARGET Arrow)
        find_package(Arrow CONFIG REQUIRED)
    endif()
    if(REFLECTCPP_BUILD_SHARED)
        target_link_libraries(reflectcpp PUBLIC Arrow::arrow_shared)
    else()
        target_link_libraries(reflectcpp PUBLIC Arrow::arrow_static)
    endif()
endif ()

if (REFLECTCPP_FLEXBUFFERS OR REFLECTCPP_CHECK_HEADERS)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_flexbuf.cpp
    )
    if (NOT TARGET flatbuffers::flatbuffers)
        find_package(flatbuffers CONFIG REQUIRED)
    endif ()
    target_link_libraries(reflectcpp PUBLIC flatbuffers::flatbuffers)
endif ()

if (REFLECTCPP_MSGPACK OR REFLECTCPP_CHECK_HEADERS)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_msgpack.cpp
    )
    find_package(msgpack-c CONFIG REQUIRED NAMES msgpack msgpack-c msgpackc)
    if(NOT REFLECTCPP_BUILD_SHARED AND TARGET msgpack-c-static)
        target_link_libraries(reflectcpp PUBLIC msgpack-c-static)
    else()
        target_link_libraries(reflectcpp PUBLIC msgpack-c)
    endif()
endif ()

if (REFLECTCPP_PARQUET OR REFLECTCPP_CHECK_HEADERS)
    if (NOT TARGET Arrow)
        find_package(Arrow CONFIG REQUIRED)
    endif()
    if (NOT TARGET Parquet)
        find_package(Parquet CONFIG REQUIRED)
    endif()

    if(REFLECTCPP_BUILD_SHARED)
        target_link_libraries(reflectcpp PUBLIC Arrow::arrow_shared Parquet::parquet_shared)
    else()
        target_link_libraries(reflectcpp PUBLIC Arrow::arrow_static Parquet::parquet_static)
    endif()
endif ()

if (REFLECTCPP_TOML OR REFLECTCPP_CHECK_HEADERS)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_toml.cpp
    )
    if (NOT TARGET tomlplusplus::tomlplusplus)
      find_package(tomlplusplus)
    endif()
    target_link_libraries(reflectcpp PUBLIC tomlplusplus::tomlplusplus)
endif()

if (REFLECTCPP_UBJSON OR REFLECTCPP_CHECK_HEADERS)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_ubjson.cpp
    )
    if (NOT TARGET jsoncons)
      find_package(jsoncons CONFIG REQUIRED)
    endif()
    include_directories(PUBLIC ${jsoncons_INCLUDE_DIRS})
endif ()

if (REFLECTCPP_XML OR REFLECTCPP_CHECK_HEADERS)
    list(APPEND REFLECT_CPP_SOURCES
        src/reflectcpp_xml.cpp
    )
    if (NOT TARGET pugixml::pugixml)
        find_package(pugixml CONFIG REQUIRED)
    endif ()
    target_link_libraries(reflectcpp PUBLIC pugixml::pugixml)
endif ()

if (REFLECTCPP_YAML OR REFLECTCPP_CHECK_HEADERS)
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
    add_library(reflectcpp_tests_crt INTERFACE)
    target_link_libraries(reflectcpp_tests_crt INTERFACE reflectcpp GTest::gtest_main)

    enable_testing()
    find_package(GTest CONFIG REQUIRED)
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

if(REFLECTCPP_CHECK_HEADERS)
    file(GLOB_RECURSE PROJECT_HEADERS "include/*.hpp")

    set(TEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/self_contained_tests")
    file(MAKE_DIRECTORY "${TEST_DIR}")

    foreach(HEADER_FILE ${PROJECT_HEADERS})
        if(NOT HEADER_FILE MATCHES "thirdparty")
            string(MAKE_C_IDENTIFIER ${HEADER_FILE} HEADER_NAME)
            set(TEST_SOURCE_FILE "${TEST_DIR}/test_${HEADER_NAME}.cpp")

            file(GENERATE
                OUTPUT ${TEST_SOURCE_FILE}
                CONTENT "#include \"${HEADER_FILE}\"\n"
            )

            add_library(check_header_${HEADER_NAME} "${TEST_SOURCE_FILE}")
            
            target_include_directories(check_header_${HEADER_NAME} PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}
                ${ADDITIONAL_INCLUDE_DIRS}
            )
            
            add_custom_target(check_${HEADER_NAME} ALL DEPENDS check_header_${HEADER_NAME})
        endif()
    endforeach()
endif()

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

