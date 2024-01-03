project(reflect-cpp-yaml-tests)

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(reflect-cpp-yaml-tests ${SOURCES})

target_link_libraries(reflect-cpp-yaml-tests PRIVATE reflectcpp)

