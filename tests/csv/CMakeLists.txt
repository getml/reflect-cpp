project(reflect-cpp-csv-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-csv-tests 
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-csv-tests PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)

target_include_directories(reflect-cpp-csv-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

target_link_libraries(reflect-cpp-csv-tests PRIVATE reflectcpp_tests_crt)

add_custom_command(TARGET reflect-cpp-csv-tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:reflect-cpp-csv-tests> $<TARGET_RUNTIME_DLLS:reflect-cpp-csv-tests>
  COMMAND_EXPAND_LISTS
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-csv-tests)

