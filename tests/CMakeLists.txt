set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

# Note: Adding -Wno-stringop-overflow is necessary, because of false positive warnings, see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=110498
if (MSVC)
    # /utf-8 tells MSVC the test sources are UTF-8 (default is the system code
    # page). Without it, non-ASCII characters in wide / u8 string literals are
    # mis-decoded, breaking the wstring/filepath round-trip tests. See PR #668.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++20 /utf-8")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Werror -ggdb -ftemplate-backtrace-limit=0")
endif()

if (REFLECTCPP_JSON)
    add_subdirectory(generic)
    add_subdirectory(json)
    add_subdirectory(json_c_arrays_and_inheritance)
    add_subdirectory(cli)
    add_subdirectory(env)
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

if (REFLECTCPP_CEREAL)
    add_subdirectory(cereal)
endif()

if (REFLECTCPP_CSV)
    add_subdirectory(csv)
endif()

if (REFLECTCPP_FLEXBUFFERS)
    add_subdirectory(flexbuffers)
endif()

if (REFLECTCPP_MSGPACK)
    add_subdirectory(msgpack)
endif()

if (REFLECTCPP_PARQUET)
    add_subdirectory(parquet)
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

if (REFLECTCPP_YAS)
    add_subdirectory(yas)
endif()

if (REFLECTCPP_BOOST_SERIALIZATION)
    add_subdirectory(boost_serialization)
endif()
