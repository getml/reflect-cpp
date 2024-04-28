project(reflect-cpp-flexbuffers-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-flexbuffers-tests 
    ${SOURCES}
    "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/src/gtest_main.cc"
)

target_include_directories(reflect-cpp-flexbuffers-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

target_link_libraries(
    reflect-cpp-flexbuffers-tests 
    PRIVATE 
    reflectcpp
    "${REFLECT_CPP_GTEST_LIB}"
)
