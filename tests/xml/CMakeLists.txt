project(reflect-cpp-xml-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-xml-tests 
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-xml-tests PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)

target_include_directories(reflect-cpp-xml-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

target_link_libraries(
    reflect-cpp-xml-tests 
    PRIVATE 
    "${REFLECT_CPP_GTEST_LIB}"
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-xml-tests)

# C++23
add_executable(
    reflect-cpp-xml-tests-cxx23
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-xml-tests-cxx23 PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)

target_include_directories(reflect-cpp-xml-tests-cxx23 SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

target_link_libraries(
    reflect-cpp-xml-tests-cxx23 
    PRIVATE 
    "${REFLECT_CPP_GTEST_LIB_CXX23}"
)
target_compile_features(reflect-cpp-xml-tests-cxx23 PRIVATE cxx_std_23)
gtest_discover_tests(reflect-cpp-xml-tests-cxx23)