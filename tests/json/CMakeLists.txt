project(reflect-cpp-json-tests)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(reflect-cpp-json-tests ${SOURCES})

target_link_libraries(reflect-cpp-json-tests PRIVATE reflectcpp)
