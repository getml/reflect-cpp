project(reflect-cpp-all-format-benchmarks)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-all-format-benchmarks 
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-all-format-benchmarks PRIVATE [["rfl.hpp"]] <iostream> <string> <functional> <gtest/gtest.h>)

target_include_directories(reflect-cpp-all-format-benchmarks SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
target_include_directories(reflect-cpp-all-format-benchmarks SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/tinycbor")

target_link_libraries(
    reflect-cpp-all-format-benchmarks
    PRIVATE 
    reflectcpp
)
