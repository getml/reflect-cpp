project(reflect-cpp-cereal-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-cereal-tests 
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-cereal-tests PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-stringop-overflow")
endif()

target_link_libraries(reflect-cpp-cereal-tests PRIVATE reflectcpp_tests_crt)

add_custom_command(TARGET reflect-cpp-cereal-tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:reflect-cpp-cereal-tests> $<TARGET_RUNTIME_DLLS:reflect-cpp-cereal-tests>
  COMMAND_EXPAND_LISTS
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-cereal-tests)
