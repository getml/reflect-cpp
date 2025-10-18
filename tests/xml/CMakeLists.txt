project(reflect-cpp-xml-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-xml-tests 
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-xml-tests PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)

target_include_directories(reflect-cpp-xml-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

target_link_libraries(reflect-cpp-xml-tests PRIVATE reflectcpp_tests_crt)

add_custom_command(TARGET reflect-cpp-xml-tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:reflect-cpp-xml-tests> $<TARGET_RUNTIME_DLLS:reflect-cpp-xml-tests>
  COMMAND_EXPAND_LISTS
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-xml-tests)

