#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "write_and_read.hpp"

namespace test_json_schema_size_validation {

struct TestStruct {
  template <typename T, typename... Validators>
  using SizedAnyOf = rfl::Validator<T, rfl::Size<rfl::AnyOf<Validators...>>>;
  template <typename T, typename... Validators>
  using SizedAllOf = rfl::Validator<T, rfl::Size<rfl::AllOf<Validators...>>>;
  template <typename T, auto Min, auto Max>
  using SizedMinMax = SizedAllOf<T, rfl::Minimum<Min>, rfl::Maximum<Max>>;

  SizedMinMax<std::string, 1, 2> a;
  SizedMinMax<std::vector<int>, 2, 3> b;
  SizedMinMax<std::set<int>, 3, 4> c;

  SizedAnyOf<std::string, rfl::EqualTo<1>, rfl::EqualTo<3>> d;
  SizedAnyOf<std::vector<int>, rfl::EqualTo<2>, rfl::EqualTo<4>> e;
  SizedAnyOf<std::set<int>, rfl::EqualTo<3>, rfl::EqualTo<5>> f;

  SizedAllOf<std::string, rfl::EqualTo<1>, rfl::EqualTo<3>> g;
  SizedAllOf<std::vector<int>, rfl::EqualTo<2>, rfl::EqualTo<4>> h;
  SizedAllOf<std::set<int>, rfl::EqualTo<3>, rfl::EqualTo<5>> i;

  std::array<int, 3> j;
};  

TEST(json, test_json_schema_size_validation) {
  constexpr auto expected_schema =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/definitions/test_json_schema_size_validation__TestStruct","definitions":{"test_json_schema_size_validation__TestStruct":{"type":"object","properties":{"a":{"allOf":[{"type":"string","minLength":1},{"type":"string","maxLength":2}]},"b":{"allOf":[{"type":"array","items":{"type":"integer"},"minItems":2},{"type":"array","items":{"type":"integer"},"maxItems":3}]},"c":{"allOf":[{"type":"array","items":{"type":"integer"},"minItems":3},{"type":"array","items":{"type":"integer"},"maxItems":4}]},"d":{"anyOf":[{"type":"string","minLength":1,"maxLength":1},{"type":"string","minLength":3,"maxLength":3}]},"e":{"anyOf":[{"type":"array","items":{"type":"integer"},"minItems":2,"maxItems":2},{"type":"array","items":{"type":"integer"},"minItems":4,"maxItems":4}]},"f":{"anyOf":[{"type":"array","items":{"type":"integer"},"minItems":3,"maxItems":3},{"type":"array","items":{"type":"integer"},"minItems":5,"maxItems":5}]},"g":{"allOf":[{"type":"string","minLength":1,"maxLength":1},{"type":"string","minLength":3,"maxLength":3}]},"h":{"allOf":[{"type":"array","items":{"type":"integer"},"minItems":2,"maxItems":2},{"type":"array","items":{"type":"integer"},"minItems":4,"maxItems":4}]},"i":{"allOf":[{"type":"array","items":{"type":"integer"},"minItems":3,"maxItems":3},{"type":"array","items":{"type":"integer"},"minItems":5,"maxItems":5}]},"j":{"type":"array","items":{"type":"integer"},"minItems":3,"maxItems":3}},"required":["a","b","c","d","e","f","g","h","i","j"]}}})";
  ASSERT_EQ(expected_schema, rfl::json::to_schema<TestStruct>());
}
}  // namespace test_json_schema_size_validation
