project(reflect-cpp-ubjson-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-ubjson-tests 
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-ubjson-tests PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)

target_link_libraries(reflect-cpp-ubjson-tests PRIVATE reflectcpp_tests_crt)

add_custom_command(TARGET reflect-cpp-ubjson-tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:reflect-cpp-ubjson-tests> $<TARGET_RUNTIME_DLLS:reflect-cpp-ubjson-tests>
  COMMAND_EXPAND_LISTS
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-ubjson-tests)

