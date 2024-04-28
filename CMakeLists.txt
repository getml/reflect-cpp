cmake_minimum_required(VERSION 3.23)

option(REFLECTCPP_BUILD_SHARED "Build shared library" ${BUILD_SHARED_LIBS})
option(REFLECTCPP_BSON "Enable BSON support" OFF)
option(REFLECTCPP_CBOR "Enable CBOR support" OFF)
option(REFLECTCPP_FLEXBUFFERS "Enable flexbuffers support" OFF)
option(REFLECTCPP_MSGPACK "Enable msgpack support" OFF)
option(REFLECTCPP_XML "Enable XML support" OFF)
option(REFLECTCPP_TOML "Enable TOML support" OFF)
option(REFLECTCPP_YAML "Enable YAML support" OFF)

option(REFLECTCPP_BUILD_TESTS "Build tests" OFF)

set(REFLECTCPP_USE_VCPKG_DEFAULT OFF)
if (REFLECTCPP_BUILD_TESTS OR REFLECTCPP_BSON OR REFLECTCPP_CBOR OR REFLECTCPP_FLEXBUFFERS OR REFLECTCPP_MSGPACK OR REFLECTCPP_XML OR REFLECTCPP_TOML OR REFLECTCPP_YAML)
    # enable vcpkg per default if require features other than JSON
    set(REFLECTCPP_USE_VCPKG_DEFAULT ON)
endif()

option(REFLECTCPP_USE_VCPKG "Use VCPKG to download and build dependencies" ${REFLECTCPP_USE_VCPKG_DEFAULT})

if (REFLECTCPP_USE_VCPKG)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
endif ()

project(reflectcpp)

set(CMAKE_CXX_STANDARD 20)

if (REFLECTCPP_BUILD_SHARED)
    add_library(reflectcpp SHARED src/yyjson.c)
else ()
    add_library(reflectcpp STATIC src/yyjson.c)
endif ()

target_compile_features(reflectcpp PUBLIC cxx_std_20)

target_include_directories(reflectcpp PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include> )

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

if (REFLECTCPP_MSGPACK)
    find_package(msgpack-c CONFIG REQUIRED)
    if (MSVC)
        target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/msgpack-c${CMAKE_STATIC_LIBRARY_SUFFIX}")
    else ()
        target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libmsgpack-c${CMAKE_STATIC_LIBRARY_SUFFIX}")
    endif ()
endif ()

if (REFLECTCPP_TOML)
    if (MSVC)
        target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/tomlplusplus${CMAKE_STATIC_LIBRARY_SUFFIX}")
    else ()
        target_link_libraries(reflectcpp PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libtomlplusplus${CMAKE_STATIC_LIBRARY_SUFFIX}")
    endif ()
endif()

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
    if (MSVC)
        set(REFLECT_CPP_GTEST_LIB "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/gtest${CMAKE_STATIC_LIBRARY_SUFFIX}")
    else ()
        set(REFLECT_CPP_GTEST_LIB "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libgtest${CMAKE_STATIC_LIBRARY_SUFFIX}")
    endif ()
    add_subdirectory(tests)
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
