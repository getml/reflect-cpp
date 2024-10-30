project(reflect-cpp-generic-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-generic-tests 
    ${SOURCES}
)

target_include_directories(reflect-cpp-generic-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

target_link_libraries(
    reflect-cpp-generic-tests 
    PRIVATE 
    "${REFLECT_CPP_GTEST_LIB}"
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-generic-tests)
