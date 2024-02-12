#include "test_all_of.hpp"
#include "test_anonymous_fields.hpp"
#include "test_box.hpp"
#include "test_custom_class1.hpp"
#include "test_custom_class2.hpp"
#include "test_custom_class3.hpp"
#include "test_custom_constructor.hpp"
#include "test_deque.hpp"
#include "test_field_variant.hpp"
#include "test_flatten_anonymous.hpp"
#include "test_forward_list.hpp"
#include "test_list.hpp"
#include "test_literal.hpp"
#include "test_map.hpp"
#include "test_monster_example1.hpp"
#include "test_monster_example2.hpp"
#include "test_multimap.hpp"
#include "test_multiset.hpp"
#include "test_optional_fields.hpp"
#include "test_readme_example.hpp"
#include "test_ref.hpp"
#include "test_save_load.hpp"
#include "test_set.hpp"
#include "test_string_map.hpp"
#include "test_string_unordered_map.hpp"
#include "test_tagged_union.hpp"
#include "test_unique_ptr.hpp"
#include "test_unique_ptr2.hpp"
#include "test_unordered_map.hpp"
#include "test_unordered_multimap.hpp"
#include "test_unordered_multiset.hpp"
#include "test_unordered_set.hpp"
#include "test_variant.hpp"
#include "test_wstring.hpp"

int main() {
  test_readme_example::test();
  test_monster_example1::test();
  test_monster_example2::test();
  test_anonymous_fields::test();
  test_flatten_anonymous::test();
  test_optional_fields::test();
  test_unique_ptr::test();
  test_unique_ptr2::test();
  test_literal::test();
  test_variant::test();
  test_field_variant::test();
  test_tagged_union::test();
  test_custom_class1::test();
  test_custom_class2::test();
  test_custom_class3::test();
  test_box::test();
  test_ref::test();

  test_deque::test();
  test_forward_list::test();
  test_list::test();
  test_map::test();
  test_multimap::test();
  test_multiset::test();
  test_set::test();
  test_string_map::test();
  test_string_unordered_map::test();
  test_unordered_map::test();
  test_unordered_multimap::test();
  test_unordered_multiset::test();
  test_unordered_set::test();

  test_custom_constructor::test();

  test_all_of::test();

  test_save_load::test();
  test_wstring::test();

  return 0;
}
