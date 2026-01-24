cmake_minimum_required(VERSION 3.28)

add_executable(reflect-cpp-modules-tests)

target_include_directories(reflect-cpp-modules-tests PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

target_sources(reflect-cpp-modules-tests PUBLIC ${SOURCES})

target_link_libraries(reflect-cpp-modules-tests PUBLIC reflectcpp_module reflectcpp_tests_crt)

find_package(GTest)

gtest_discover_tests(reflect-cpp-modules-tests)
