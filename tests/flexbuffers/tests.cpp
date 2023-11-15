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

int main() {
  test_readme_example();
  test_monster_example1();
  test_monster_example2();
  test_anonymous_fields();
  test_flatten_anonymous();
  test_optional_fields();
  test_unique_ptr();
  test_unique_ptr2();
  test_literal();
  test_variant();
  test_field_variant();
  test_tagged_union();
  test_custom_class1();
  test_custom_class2();
  test_custom_class3();
  test_box();
  test_ref();

  test_deque();
  test_forward_list();
  test_list();
  test_map();
  test_multimap();
  test_multiset();
  test_set();
  test_string_map();
  test_string_unordered_map();
  test_unordered_map();
  test_unordered_multimap();
  test_unordered_multiset();
  test_unordered_set();

  test_custom_constructor();

  test_all_of();

  test_save_load();

  return 0;
}
