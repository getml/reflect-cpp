set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++20")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wno-sign-compare -Wno-missing-braces -Wno-psabi -pthread -fno-strict-aliasing -fwrapv -O2 -ftemplate-backtrace-limit=0 -fsanitize=undefined")
endif()

add_subdirectory(json)

if (REFLECTCPP_BSON)
    add_subdirectory(bson)
endif()

if (REFLECTCPP_CBOR)
    add_subdirectory(cbor)
endif()

if (REFLECTCPP_FLEXBUFFERS)
    add_subdirectory(flexbuffers)
endif()

if (REFLECTCPP_MSGPACK)
    add_subdirectory(msgpack)
endif()

if (REFLECTCPP_XML)
    add_subdirectory(xml)
endif()

if (REFLECTCPP_TOML)
    add_subdirectory(toml)
endif()

if (REFLECTCPP_YAML)
    add_subdirectory(yaml)
endif()
