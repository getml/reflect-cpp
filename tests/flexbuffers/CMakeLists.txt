project(reflect-cpp-json-tests)

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(reflect-cpp-flatbuffers-tests ${SOURCES})

target_link_libraries(reflect-cpp-flatbuffers-tests PRIVATE reflectcpp)
