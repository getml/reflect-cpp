project(reflect-cpp-json-c-arrays-and-inheritance-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-json-c-arrays-and-inheritance-tests 
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-json-c-arrays-and-inheritance-tests PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DREFLECT_CPP_C_ARRAYS_OR_INHERITANCE")

target_include_directories(reflect-cpp-json-c-arrays-and-inheritance-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

target_link_libraries(reflect-cpp-json-c-arrays-and-inheritance-tests PRIVATE reflectcpp_tests_crt)

add_custom_command(TARGET reflect-cpp-json-c-arrays-and-inheritance-tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:reflect-cpp-json-c-arrays-and-inheritance-tests> $<TARGET_RUNTIME_DLLS:reflect-cpp-json-c-arrays-and-inheritance-tests>
  COMMAND_EXPAND_LISTS
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-json-c-arrays-and-inheritance-tests)

