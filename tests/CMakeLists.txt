set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++20")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -ggdb -ftemplate-backtrace-limit=0")
endif()

if (REFLECTCPP_JSON)
    add_subdirectory(generic)
    add_subdirectory(json)
    add_subdirectory(json_c_arrays_and_inheritance)
endif ()

if (REFLECTCPP_AVRO)
    add_subdirectory(avro)
endif()

if (REFLECTCPP_BSON)
    add_subdirectory(bson)
endif()

if (REFLECTCPP_CAPNPROTO)
    add_subdirectory(capnproto)
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

if (REFLECTCPP_TOML)
    add_subdirectory(toml)
endif()

if (REFLECTCPP_UBJSON)
    add_subdirectory(ubjson)
endif()

if (REFLECTCPP_XML)
    add_subdirectory(xml)
endif()

if (REFLECTCPP_YAML)
    add_subdirectory(yaml)
endif()
