cmake_minimum_required(VERSION 3.28)

add_executable(reflect-cpp-modules-tests)

if(REFLECTCPP_JSON)
    target_compile_definitions(reflect-cpp-modules-tests PUBLIC REFLECTCPP_JSON)
endif()

if(REFLECTCPP_AVRO)
    target_compile_definitions(reflect-cpp-modules-tests PUBLIC REFLECTCPP_AVRO)
endif()

if(REFLECTCPP_BSON)
    target_compile_definitions(reflect-cpp-modules-tests PUBLIC REFLECTCPP_BSON)
endif()

if(REFLECTCPP_CAPNPROTO)
    target_compile_definitions(reflect-cpp-modules-tests PUBLIC REFLECTCPP_CAPNPROTO)
endif()

if(REFLECTCPP_CBOR)
    target_compile_definitions(reflect-cpp-modules-tests PUBLIC REFLECTCPP_CBOR)
endif()

if(REFLECTCPP_CSV)
    target_compile_definitions(reflect-cpp-modules-tests PUBLIC REFLECTCPP_CSV)
endif()

if(REFLECTCPP_FLEXBUFFERS)
    target_compile_definitions(reflect-cpp-modules-tests PUBLIC REFLECTCPP_FLEXBUFFERS)
endif()

if(REFLECTCPP_MSGPACK)
    target_compile_definitions(reflect-cpp-modules-tests PUBLIC REFLECTCPP_MSGPACK)
endif()

if(REFLECTCPP_PARQUET)
    target_compile_definitions(reflect-cpp-modules-tests PUBLIC REFLECTCPP_PARQUET)
endif()

if(REFLECTCPP_TOML)
    target_compile_definitions(reflect-cpp-modules-tests PUBLIC REFLECTCPP_TOML)
endif()

if(REFLECTCPP_UBJSON)
    target_compile_definitions(reflect-cpp-modules-tests PUBLIC REFLECTCPP_UBJSON)
endif()

if(REFLECTCPP_XML)
    target_compile_definitions(reflect-cpp-modules-tests PUBLIC REFLECTCPP_XML)
endif()

if(REFLECTCPP_YAML)
    target_compile_definitions(reflect-cpp-modules-tests PUBLIC REFLECTCPP_YAML)
endif()


target_include_directories(reflect-cpp-modules-tests PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

target_sources(reflect-cpp-modules-tests PUBLIC ${SOURCES})

target_link_libraries(reflect-cpp-modules-tests PUBLIC reflectcpp_module reflectcpp_tests_crt)

find_package(GTest)

gtest_discover_tests(reflect-cpp-modules-tests)
