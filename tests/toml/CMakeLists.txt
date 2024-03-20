project(reflect-cpp-toml-tests)

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(reflect-cpp-toml-tests ${SOURCES})

target_link_libraries(reflect-cpp-toml-tests PRIVATE reflectcpp)

