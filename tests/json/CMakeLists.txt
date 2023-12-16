cmake_minimum_required(VERSION 3.4)

project(reflect-cpp-tests)

include_directories(
    "../../include"
)

add_executable(
    reflect-cpp-tests
    "tests.cpp"
    "test_all_of.cpp"
    "test_alphanumeric_map.cpp"
    "test_alphanumeric_unordered_map.cpp"
    "test_anonymous_fields.cpp"
    "test_any_of.cpp"
    "test_array.cpp"
    "test_as.cpp"
    "test_as2.cpp"
    "test_as_flatten.cpp"
    "test_box.cpp"
    "test_custom_class1.cpp"
    "test_custom_class2.cpp"
    "test_custom_class3.cpp"
    "test_custom_class4.cpp"
    "test_custom_constructor.cpp"
    "test_default_values.cpp"
    "test_deque.cpp"
    "test_email.cpp"
    "test_enum.cpp"
    "test_error_messages.cpp"
    "test_field_variant.cpp"
    "test_flag_enum.cpp"
    "test_flag_enum_with_int.cpp"
    "test_flatten.cpp"
    "test_flatten_anonymous.cpp"
    "test_forward_list.cpp"
    "test_list.cpp"
    "test_literal.cpp"
    "test_literal_map.cpp"
    "test_literal_unordered_map.cpp"
    "test_map.cpp"
    "test_map_with_key_validation.cpp"
    "test_meta_fields.cpp"
    "test_monster_example.cpp"
    "test_move_replace.cpp"
    "test_multimap.cpp"
    "test_multiset.cpp"
    "test_one_of.cpp"
    "test_optional_fields.cpp"
    "test_readme_example.cpp"
    "test_ref.cpp"
    "test_replace.cpp"
    "test_replace2.cpp"
    "test_replace_flatten.cpp"
    "test_replace_flatten2.cpp"
    "test_replace_with_other_struct.cpp"
    "test_replace_with_other_struct2.cpp"
    "test_result.cpp"
    "test_save_load.cpp"
    "test_set.cpp"
    "test_size.cpp"
    "test_std_ref.cpp"
    "test_string_map.cpp"
    "test_string_unordered_map.cpp"
    "test_tagged_union.cpp"
    "test_tagged_union2.cpp"
    "test_timestamp.cpp"
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

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++20")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wno-sign-compare -Wno-missing-braces -Wno-psabi -pthread -fno-strict-aliasing -fwrapv -O2 -ftemplate-backtrace-limit=0 -fsanitize=undefined")
endif()

