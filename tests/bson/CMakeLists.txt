project(reflect-cpp-bson-tests)

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(reflect-cpp-bson-tests ${SOURCES})

target_include_directories(reflect-cpp-bson-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
target_link_libraries(reflect-cpp-bson-tests PRIVATE reflectcpp)
