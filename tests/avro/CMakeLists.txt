project(reflect-cpp-avro-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-avro-tests 
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-avro-tests PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)


target_link_libraries(reflect-cpp-avro-tests PRIVATE reflectcpp_tests_crt)

add_custom_command(TARGET reflect-cpp-avro-tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:reflect-cpp-avro-tests> $<TARGET_RUNTIME_DLLS:reflect-cpp-avro-tests>
  COMMAND_EXPAND_LISTS
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-avro-tests)
