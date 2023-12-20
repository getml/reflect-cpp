project(reflect-cpp-flexbuffer-tests)

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(reflect-cpp-flexbuffers-tests ${SOURCES})

target_link_libraries(reflect-cpp-flexbuffers-tests PRIVATE reflectcpp)
