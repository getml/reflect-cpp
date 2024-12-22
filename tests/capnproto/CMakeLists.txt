project(reflect-cpp-capnproto-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-capnproto-tests 
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-capnproto-tests PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)


target_link_libraries(
    reflect-cpp-capnproto-tests 
    PRIVATE 
    "${REFLECT_CPP_GTEST_LIB}"
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-capnproto-tests)
