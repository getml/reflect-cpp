project(reflect-cpp-avro-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-avro-tests 
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-avro-tests PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)


target_link_libraries(
    reflect-cpp-avro-tests 
    PRIVATE 
    "${REFLECT_CPP_GTEST_LIB}"
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-avro-tests)
