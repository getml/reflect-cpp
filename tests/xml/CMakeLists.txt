project(reflect-cpp-xml-tests)

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(reflect-cpp-xml-tests ${SOURCES})

target_link_libraries(reflect-cpp-xml-tests PRIVATE reflectcpp)

