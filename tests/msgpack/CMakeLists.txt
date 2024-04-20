project(reflect-cpp-msgpack-tests)

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(reflect-cpp-msgpack-tests ${SOURCES})

target_include_directories(reflect-cpp-msgpack-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
target_link_libraries(reflect-cpp-msgpack-tests PRIVATE reflectcpp)
