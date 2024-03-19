#include "test_array.hpp"
#include "test_box.hpp"
#include "test_custom_class1.hpp"
#include "test_custom_class3.hpp"
#include "test_custom_class4.hpp"
#include "test_default_values.hpp"
#include "test_deque.hpp"
#include "test_enum.hpp"
#include "test_field_variant.hpp"
#include "test_flag_enum.hpp"
#include "test_flag_enum_with_int.hpp"
#include "test_flatten.hpp"
#include "test_flatten_anonymous.hpp"
#include "test_forward_list.hpp"
#include "test_literal.hpp"
#include "test_literal_map.hpp"
#include "test_map.hpp"
#include "test_map_with_key_validation.hpp"
#include "test_monster_example.hpp"
#include "test_oid.hpp"
#include "test_readme_example.hpp"
#include "test_readme_example2.hpp"
#include "test_ref.hpp"
#include "test_save_load.hpp"
#include "test_set.hpp"
#include "test_size.hpp"
#include "test_tagged_union.hpp"
#include "test_timestamp.hpp"
#include "test_unique_ptr.hpp"
#include "test_unique_ptr2.hpp"
#include "test_variant.hpp"
#include "test_wstring.hpp"

int main() {
  test_readme_example::test();
  test_readme_example2::test();
  test_flatten::test();
  test_flatten_anonymous::test();
  test_enum::test();
  test_flag_enum::test();
  test_flag_enum_with_int::test();
  test_map::test();
  test_map_with_key_validation::test();
  test_variant::test();
  test_field_variant::test();
  test_tagged_union::test();
  test_deque::test();
  test_forward_list::test();
  test_literal_map::test();
  test_literal::test();
  test_monster_example::test();
  test_ref::test();
  test_set::test();
  test_size::test();
  test_timestamp::test();
  test_unique_ptr::test();
  test_unique_ptr2::test();
  test_array::test();
  test_box::test();
  test_custom_class1::test();
  test_custom_class3::test();
  test_custom_class4::test();
  test_default_values::test();
  test_save_load::test();
  test_wstring::test();
  test_oid::test();

  return 0;
}
