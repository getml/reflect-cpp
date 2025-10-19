project(reflect-cpp-generic-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-generic-tests 
    ${SOURCES}
)

target_include_directories(reflect-cpp-generic-tests SYSTEM PRIVATE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

target_link_libraries(reflect-cpp-generic-tests PRIVATE reflectcpp_tests_crt)

add_custom_command(TARGET reflect-cpp-generic-tests POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:reflect-cpp-generic-tests> $<TARGET_RUNTIME_DLLS:reflect-cpp-generic-tests>
  COMMAND_EXPAND_LISTS
)

find_package(GTest)
gtest_discover_tests(reflect-cpp-generic-tests)

