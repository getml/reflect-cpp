#include "test_all_of.hpp"
#include "test_anonymous_fields.hpp"
#include "test_field_variant.hpp"
#include "test_literal.hpp"
#include "test_optional_fields.hpp"
#include "test_readme_example.hpp"
#include "test_tagged_union.hpp"
#include "test_unique_ptr.hpp"
#include "test_unique_ptr2.hpp"
#include "test_variant.hpp"

int main() {
  test_readme_example();
  test_anonymous_fields();
  test_optional_fields();
  test_unique_ptr();
  test_unique_ptr2();
  test_literal();
  test_variant();
  test_field_variant();
  test_tagged_union();

  test_all_of();

  return 0;
}
