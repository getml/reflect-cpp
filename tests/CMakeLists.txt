set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

# Generate one adapter for each enabled module. The same test sources can then
# exercise either headers or module imports without preprocessor switches.
foreach(MODULE_FILE IN LISTS REFLECT_CPP_MODULES)
    get_filename_component(MODULE_NAME "${MODULE_FILE}" NAME_WLE)
    if(REFLECTCPP_BUILD_MODULES)
        set(REFLECTCPP_TEST_DEPENDENCY "import ${MODULE_NAME};")
    elseif(MODULE_NAME STREQUAL "rfl")
        set(REFLECTCPP_TEST_DEPENDENCY "#include <rfl.hpp>\n#include <rfl/Deprecated.hpp>\n#include <rfl/num_fields.hpp>")
    elseif(MODULE_NAME STREQUAL "rfl.c_arrays_and_inheritance")
        set(REFLECTCPP_TEST_DEPENDENCY "#define REFLECT_CPP_C_ARRAYS_OR_INHERITANCE\n#include <rfl.hpp>\n#include <rfl/json.hpp>\n#include <rfl/num_fields.hpp>")
    elseif(MODULE_NAME STREQUAL "rfl.ctre")
        continue()
    else()
        string(REPLACE "." "/" HEADER_NAME "${MODULE_NAME}")
        set(REFLECTCPP_TEST_DEPENDENCY "#include <${HEADER_NAME}.hpp>")
    endif()
    configure_file(test_rfl.hpp.in "test_rfl/${MODULE_NAME}.hpp" @ONLY)
endforeach()

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
endif ()

if (REFLECTCPP_CLI)
    add_subdirectory(cli)
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
