project(reflect-cpp-boost-serialization-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-boost-serialization-tests
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-boost-serialization-tests PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)


target_link_libraries(reflect-cpp-boost-serialization-tests PRIVATE reflectcpp_tests_crt)

add_custom_command(TARGET reflect-cpp-boost-serialization-tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:reflect-cpp-boost-serialization-tests> $<TARGET_RUNTIME_DLLS:reflect-cpp-boost-serialization-tests>
  COMMAND_EXPAND_LISTS
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-boost-serialization-tests)
