cmake_minimum_required(VERSION 3.4)

project(reflect-cpp-tests)

include_directories(
    "../../include"
    "../../../flatbuffers/include"
)

add_executable(
    reflect-cpp-tests
    "tests.cpp"
    "test_all_of.cpp"
    "test_anonymous_fields.cpp"
    "test_box.cpp"
    "test_custom_class1.cpp"
    "test_custom_class2.cpp"
    "test_custom_class3.cpp"
    "test_custom_constructor.cpp"
    "test_deque.cpp"
    "test_field_variant.cpp"
    "test_flatten_anonymous.cpp"
    "test_forward_list.cpp"
    "test_list.cpp"
    "test_literal.cpp"
    "test_map.cpp"
    "test_monster_example1.cpp"
    "test_monster_example2.cpp"
    "test_multimap.cpp"
    "test_multiset.cpp"
    "test_optional_fields.cpp"
    "test_readme_example.cpp"
    "test_ref.cpp"
    "test_save_load.cpp"
    "test_set.cpp"
    "test_string_map.cpp"
    "test_string_unordered_map.cpp"
    "test_tagged_union.cpp"
    "test_unique_ptr.cpp"
    "test_unique_ptr2.cpp"
    "test_unordered_map.cpp"
    "test_unordered_multimap.cpp"
    "test_unordered_multiset.cpp"
    "test_unordered_set.cpp"
    "test_variant.cpp"
    "../../src/yyjson.c"
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wno-sign-compare -Wno-missing-braces -Wno-psabi -pthread -fno-strict-aliasing -fwrapv -O2 -ftemplate-backtrace-limit=0")

target_link_libraries(
    reflect-cpp-tests
    /home/reflectcpp/flatbuffers/build/libflatbuffers.a
)
