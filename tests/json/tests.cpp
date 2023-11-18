#include "test_all_of.hpp"
#include "test_anonymous_fields.hpp"
#include "test_any_of.hpp"
#include "test_array.hpp"
#include "test_as.hpp"
#include "test_as_flatten.hpp"
#include "test_box.hpp"
#include "test_custom_class1.hpp"
#include "test_custom_class2.hpp"
#include "test_custom_class3.hpp"
#include "test_custom_class4.hpp"
#include "test_custom_constructor.hpp"
#include "test_default_values.hpp"
#include "test_deque.hpp"
#include "test_email.hpp"
#include "test_error_messages.hpp"
#include "test_field_variant.hpp"
#include "test_flatten.hpp"
#include "test_flatten_anonymous.hpp"
#include "test_flatten_tuple.hpp"
#include "test_forward_list.hpp"
#include "test_list.hpp"
#include "test_literal.hpp"
#include "test_literal_map.hpp"
#include "test_literal_unordered_map.hpp"
#include "test_map.hpp"
#include "test_monster_example.hpp"
#include "test_multimap.hpp"
#include "test_multiset.hpp"
#include "test_one_of.hpp"
#include "test_optional_fields.hpp"
#include "test_readme_example.hpp"
#include "test_ref.hpp"
#include "test_replace.hpp"
#include "test_replace_flatten.hpp"
#include "test_replace_with_other_struct.hpp"
#include "test_result.hpp"
#include "test_save_load.hpp"
#include "test_set.hpp"
#include "test_size.hpp"
#include "test_string_map.hpp"
#include "test_string_unordered_map.hpp"
#include "test_tagged_union.hpp"
#include "test_timestamp.hpp"
#include "test_unique_ptr.hpp"
#include "test_unique_ptr2.hpp"
#include "test_unordered_map.hpp"
#include "test_unordered_multimap.hpp"
#include "test_unordered_multiset.hpp"
#include "test_unordered_set.hpp"
#include "test_variant.hpp"

int main() {
  test_readme_example();
  test_default_values();
  test_optional_fields();
  test_unique_ptr();
  test_unique_ptr2();
  test_literal();
  test_variant();
  test_tagged_union();
  test_field_variant();
  test_ref();
  test_box();
  test_array();
  test_timestamp();
  test_flatten();
  test_flatten_anonymous();
  test_flatten_tuple();
  test_deque();
  test_forward_list();
  test_list();
  test_map();
  test_unordered_map();
  test_set();
  test_unordered_set();
  test_multimap();
  test_unordered_multimap();
  test_multiset();
  test_unordered_multiset();
  test_string_map();
  test_string_unordered_map();
  test_literal_map();
  test_literal_unordered_map();
  test_error_messages();
  test_result();
  test_anonymous_fields();
  test_monster_example();

  test_custom_class1();
  test_custom_class2();
  test_custom_class3();
  test_custom_class4();

  test_replace();
  test_replace_flatten();
  test_replace_with_other_struct();
  test_as();
  test_as_flatten();

  test_custom_constructor();

  test_all_of();
  test_any_of();
  test_one_of();
  test_email();
  test_size();

  test_save_load();

  return 0;
}
