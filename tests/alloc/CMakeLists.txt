project(reflect-cpp-alloc-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-alloc-tests
    ${SOURCES}
)

target_include_directories(reflect-cpp-alloc-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

target_link_libraries(reflect-cpp-alloc-tests PRIVATE reflectcpp_tests_crt)

add_custom_command(TARGET reflect-cpp-alloc-tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:reflect-cpp-alloc-tests> $<TARGET_RUNTIME_DLLS:reflect-cpp-alloc-tests>
  COMMAND_EXPAND_LISTS
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-alloc-tests)
