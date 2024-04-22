project(reflect-cpp-cbor-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-cbor-tests 
    ${SOURCES}
    "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/src/gtest_main.cc"
)

target_include_directories(reflect-cpp-cbor-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
target_include_directories(reflect-cpp-cbor-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/tinycbor")

target_link_libraries(
    reflect-cpp-cbor-tests 
    PRIVATE 
    reflectcpp
    "${REFLECT_CPP_GTEST_LIB}"
)
