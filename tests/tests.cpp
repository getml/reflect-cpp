#include "test_array.hpp"
#include "test_custom_class1.hpp"
#include "test_custom_class2.hpp"
#include "test_custom_class3.hpp"
#include "test_default_values.hpp"
#include "test_field_variant.hpp"
#include "test_literal.hpp"
#include "test_optional_fields.hpp"
#include "test_readme_example.hpp"
#include "test_ref.hpp"
#include "test_tagged_union.hpp"
#include "test_timestamp.hpp"
#include "test_variant.hpp"

int main() {
    test_readme_example();
    test_default_values();
    test_optional_fields();
    test_literal();
    test_variant();
    test_tagged_union();
    test_field_variant();
    test_ref();
    test_custom_class1();
    test_custom_class2();
    test_custom_class3();
    test_array();
    test_timestamp();

    return 0;
}
