project(reflect-cpp-capnproto-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*/compile_fail/.*")

add_executable(
    reflect-cpp-capnproto-tests 
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-capnproto-tests PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)


target_link_libraries(reflect-cpp-capnproto-tests PRIVATE reflectcpp_tests_crt)

add_custom_command(TARGET reflect-cpp-capnproto-tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:reflect-cpp-capnproto-tests> $<TARGET_RUNTIME_DLLS:reflect-cpp-capnproto-tests>
  COMMAND_EXPAND_LISTS
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-capnproto-tests)

# read(istream, schema) has infinite recursion → compile error.
# This test tries to compile the buggy overload.
# WILL_FAIL: test passes when compilation fails (bug present),
#            test fails when compilation succeeds (bug fixed — remove WILL_FAIL).
add_test(
  NAME capnproto_regression.read_istream_schema_should_compile
  COMMAND ${CMAKE_COMMAND}
    --build ${CMAKE_BINARY_DIR}
    --target capnproto-compile-fail-read-istream-schema
)
set_tests_properties(
  capnproto_regression.read_istream_schema_should_compile
  PROPERTIES WILL_FAIL TRUE
)
add_library(capnproto-compile-fail-read-istream-schema OBJECT EXCLUDE_FROM_ALL
  compile_fail/capnproto_read_istream_schema.cpp
)
target_link_libraries(capnproto-compile-fail-read-istream-schema PRIVATE reflectcpp_tests_crt)
