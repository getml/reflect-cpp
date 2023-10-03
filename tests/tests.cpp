#include "test_default_values.hpp"
#include "test_field_variant.hpp"
#include "test_literal.hpp"
#include "test_readme_example.hpp"
#include "test_ref.hpp"
#include "test_tagged_union.hpp"
#include "test_variant.hpp"

int main() {
    test_readme_example();
    test_default_values();
    test_literal();
    test_variant();
    test_tagged_union();
    test_field_variant();
    test_ref();

    return 0;
}
