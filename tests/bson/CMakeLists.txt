project(reflect-cpp-bson-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-bson-tests 
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-bson-tests PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)

target_include_directories(reflect-cpp-bson-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

target_link_libraries(reflect-cpp-bson-tests PRIVATE reflectcpp_tests_crt)

add_custom_command(TARGET reflect-cpp-bson-tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:reflect-cpp-bson-tests> $<TARGET_RUNTIME_DLLS:reflect-cpp-bson-tests>
  COMMAND_EXPAND_LISTS
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-bson-tests)
