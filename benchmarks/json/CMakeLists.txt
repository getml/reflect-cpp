project(reflect-cpp-json-benchmarks)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(
    reflect-cpp-json-benchmarks
    ${SOURCES}
)
target_precompile_headers(reflect-cpp-json-benchmarks PRIVATE [["rfl.hpp"]] <iostream> <string> <functional>)

target_link_libraries(
    reflect-cpp-json-benchmarks
    PRIVATE
    reflectcpp
)

if (REFLECTCPP_USE_GPERFTOOLS)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PC_GPERFTOOLS REQUIRED libprofiler libtcmalloc)
    target_include_directories(reflect-cpp-json-benchmarks SYSTEM PRIVATE ${PC_GPERFTOOLS_INCLUDE_DIRS})
    target_link_directories(reflect-cpp-json-benchmarks PRIVATE ${PC_GPERFTOOLS_LIBRARY_DIRS})
    target_link_libraries(reflect-cpp-json-benchmarks PRIVATE ${PC_GPERFTOOLS_LIBRARIES})
    target_link_options(reflect-cpp-json-benchmarks PRIVATE ${PC_GPERFTOOLS_LDFLAGS})
endif ()
