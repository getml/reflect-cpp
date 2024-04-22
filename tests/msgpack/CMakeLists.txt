project(reflect-cpp-msgpack-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-msgpack-tests 
    ${SOURCES}
    "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/src/gtest_main.cc"
)

target_include_directories(reflect-cpp-msgpack-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

target_link_libraries(
    reflect-cpp-msgpack-tests 
    PRIVATE 
    reflectcpp
    "${REFLECT_CPP_GTEST_LIB}"
)
