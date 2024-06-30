set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++20 -O2")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -O2")
endif()

add_subdirectory(all)
add_subdirectory(json)
