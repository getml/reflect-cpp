project(reflect-cpp-json-benchmarks)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-json-benchmarks 
    ${SOURCES}
)

target_include_directories(reflect-cpp-json-benchmarks SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

target_link_libraries(
    reflect-cpp-json-benchmarks
    PRIVATE 
    reflectcpp
)
