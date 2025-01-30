project(reflect-cpp-cbor-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-cbor-tests 
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-cbor-tests PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)

target_link_libraries(
    reflect-cpp-cbor-tests 
    PRIVATE 
    "${REFLECT_CPP_GTEST_LIB}"
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-cbor-tests)

# C++ 23 
add_executable(
    reflect-cpp-cbor-tests-cxx23
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-cbor-tests-cxx23 PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)

target_include_directories(reflect-cpp-cbor-tests-cxx23 SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
target_include_directories(reflect-cpp-cbor-tests-cxx23 SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/tinycbor")

target_link_libraries(
    reflect-cpp-cbor-tests-cxx23
    PRIVATE 
    "${REFLECT_CPP_GTEST_LIB_CXX23}"
)
target_compile_features(reflect-cpp-cbor-tests-cxx23 PUBLIC cxx_std_23)
gtest_discover_tests(reflect-cpp-cbor-tests-cxx23)