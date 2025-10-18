project(reflect-cpp-parquet-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-parquet-tests 
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-parquet-tests PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)

target_include_directories(reflect-cpp-parquet-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

target_link_libraries(reflect-cpp-parquet-tests PRIVATE reflectcpp_tests_crt)

add_custom_command(TARGET reflect-cpp-parquet-tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:reflect-cpp-parquet-tests> $<TARGET_RUNTIME_DLLS:reflect-cpp-parquet-tests>
  COMMAND_EXPAND_LISTS
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-parquet-tests)

