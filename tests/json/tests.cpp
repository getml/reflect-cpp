#include "test_all_of.hpp"
#include "test_alphanumeric_map.hpp"
#include "test_alphanumeric_unordered_map.hpp"
#include "test_and_then.hpp"
#include "test_anonymous_fields.hpp"
#include "test_any_of.hpp"
#include "test_apply.hpp"
#include "test_array.hpp"
#include "test_as.hpp"
#include "test_as2.hpp"
#include "test_as_flatten.hpp"
#include "test_box.hpp"
#include "test_box2.hpp"
#include "test_c_array_class1.hpp"
#include "test_c_array_class2.hpp"
#include "test_c_array_class3.hpp"
#include "test_c_array_class4.hpp"
#include "test_c_array_class5.hpp"
#include "test_const_fields.hpp"
#include "test_custom_class1.hpp"
#include "test_custom_class2.hpp"
#include "test_custom_class3.hpp"
#include "test_custom_class4.hpp"
#include "test_custom_constructor.hpp"
#include "test_default_values.hpp"
#include "test_deque.hpp"
#include "test_email.hpp"
#include "test_empty_object.hpp"
#include "test_enum1.hpp"
#include "test_enum2.hpp"
#include "test_enum3.hpp"
#include "test_enum4.hpp"
#include "test_enum5.hpp"
#include "test_enum6.hpp"
#include "test_enum7.hpp"
#include "test_error_messages.hpp"
#include "test_field_variant.hpp"
#include "test_flag_enum1.hpp"
#include "test_flag_enum2.hpp"
#include "test_flag_enum_with_int.hpp"
#include "test_flatten.hpp"
#include "test_flatten_anonymous.hpp"
#include "test_forward_list.hpp"
#include "test_inside_function.hpp"
#include "test_json_schema.hpp"
#include "test_list.hpp"
#include "test_literal.hpp"
#include "test_literal_map.hpp"
#include "test_literal_unordered_map.hpp"
#include "test_map.hpp"
#include "test_map_with_key_validation.hpp"
#include "test_meta_fields.hpp"
#include "test_monster_example.hpp"
#include "test_multimap.hpp"
#include "test_multiset.hpp"
#include "test_one_of.hpp"
#include "test_optional_fields.hpp"
#include "test_readme_example.hpp"
#include "test_ref.hpp"
#include "test_replace.hpp"
#include "test_replace2.hpp"
#include "test_replace_flatten.hpp"
#include "test_replace_flatten2.hpp"
#include "test_replace_with_other_struct.hpp"
#include "test_replace_with_other_struct2.hpp"
#include "test_result.hpp"
#include "test_save_load.hpp"
#include "test_set.hpp"
#include "test_shared_ptr.hpp"
#include "test_size.hpp"
#include "test_std_ref.hpp"
#include "test_string_map.hpp"
#include "test_string_unordered_map.hpp"
#include "test_tagged_union.hpp"
#include "test_tagged_union2.hpp"
#include "test_tagged_union3.hpp"
#include "test_timestamp.hpp"
#include "test_transform.hpp"
#include "test_unique_ptr.hpp"
#include "test_unique_ptr2.hpp"
#include "test_unnamed_namespace.hpp"
#include "test_unordered_map.hpp"
#include "test_unordered_multimap.hpp"
#include "test_unordered_multiset.hpp"
#include "test_unordered_set.hpp"
#include "test_variant.hpp"
#include "test_view.hpp"
#include "test_wstring.hpp"
#include "test_inheritance.hpp"

int main() {
  test_readme_example::test();
  test_default_values::test();
  test_optional_fields::test();
  test_unique_ptr::test();
  test_unique_ptr2::test();
  test_shared_ptr::test();
  test_literal::test();
  test_variant::test();
  test_tagged_union::test();
  test_tagged_union2::test();
  test_tagged_union3::test();
  test_field_variant::test();
  test_ref::test();
  test_box::test();
  test_box2::test();
  test_array::test();
  test_timestamp::test();
  test_flatten::test();
  test_flatten_anonymous::test();
  test_deque::test();
  test_forward_list::test();
  test_list::test();
  test_map::test();
  test_map_with_key_validation::test();
  test_unordered_map::test();
  test_set::test();
  test_unordered_set::test();
  test_multimap::test();
  test_unordered_multimap::test();
  test_multiset::test();
  test_unordered_multiset::test();
  test_std_ref::test();
  test_string_map::test();
  test_string_unordered_map::test();
  test_alphanumeric_map::test();
  test_alphanumeric_unordered_map::test();
  test_literal_map::test();
  test_literal_unordered_map::test();
  test_error_messages::test();
  test_result::test();
  test_anonymous_fields::test();
  test_monster_example::test();
  test_unnamed_namespace::test();
  test_inside_function::test();
  test_const_fields::test();
  test_empty_object::test();

  test_enum1::test();
  test_enum2::test();
  test_enum3::test();
  test_enum4::test();
  test_enum5::test();
  test_enum6::test();
  test_enum7::test();
  test_flag_enum1::test();
  test_flag_enum2::test();
  test_flag_enum_with_int::test();

  test_custom_class1::test();
  test_custom_class2::test();
  test_custom_class3::test();
  test_custom_class4::test();

  test_c_array_class1::test();
  test_c_array_class2::test();
  test_c_array_class3::test();
  test_c_array_class4::test();
  test_c_array_class5::test();

  test_replace::test();
  test_replace2::test();
  test_replace_flatten::test();
  test_replace_flatten2::test();
  test_replace_with_other_struct::test();
  test_replace_with_other_struct2::test();
  test_as::test();
  test_as2::test();
  test_as_flatten::test();

  test_view::test();
  test_apply::test();
  test_transform::test();
  test_and_then::test();

  test_custom_constructor::test();

  test_all_of::test();
  test_any_of::test();
  test_one_of::test();
  test_email::test();
  test_size::test();

  test_save_load::test();

  test_meta_fields::test();

  test_wstring::test();
  test_json_schema::test();

  test_inheritance::test();

  return 0;
}
