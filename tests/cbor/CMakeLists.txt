project(reflect-cpp-cbor-tests)

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(reflect-cpp-cbor-tests ${SOURCES})

target_include_directories(reflect-cpp-cbor-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/tinycbor")
target_link_libraries(reflect-cpp-cbor-tests PRIVATE reflectcpp)
