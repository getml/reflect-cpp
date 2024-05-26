set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++20")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -O2 -s")
endif()

add_subdirectory(json)
