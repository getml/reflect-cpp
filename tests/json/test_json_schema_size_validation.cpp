#include <array>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <set>
#include <string>
#include <vector>

#include "gtest/gtest.h"

namespace test_json_schema_size_validation {

using rfl::EqualTo;
using rfl::Maximum;
using rfl::Minimum;
using rfl::json::to_schema;

template <typename T, typename... Validators>
using SizedAnyOf = rfl::Validator<T, rfl::Size<rfl::AnyOf<Validators...>>>;
template <typename T, typename... Validators>
using SizedAllOf = rfl::Validator<T, rfl::Size<rfl::AllOf<Validators...>>>;
template <typename T, auto Min, auto Max>
using SizedMinMax =
    rfl::Validator<T, rfl::Size<Minimum<Min>>, rfl::Size<Maximum<Max>>>;

TEST(json, test_json_schema_sized_vector_min1_max2) {
  constexpr auto expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","allOf":[{"type":"array","items":{"type":"integer"},"minItems":1},{"type":"array","items":{"type":"integer"},"maxItems":2}],"definitions":{}})";

  auto actual = to_schema<SizedMinMax<std::vector<int>, 1, 2>>();
  ASSERT_EQ(expected, actual);
  actual = to_schema<SizedAllOf<std::vector<int>, Minimum<1>, Maximum<2>>>();
  ASSERT_EQ(expected, actual);
}

TEST(json, test_json_schema_sized_set_min2_max3) {
  constexpr auto expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","allOf":[{"type":"array","items":{"type":"integer"},"minItems":2},{"type":"array","items":{"type":"integer"},"maxItems":3}],"definitions":{}})";

  auto actual = to_schema<SizedMinMax<std::set<int>, 2, 3>>();
  ASSERT_EQ(expected, actual);
  actual = to_schema<SizedAllOf<std::set<int>, Minimum<2>, Maximum<3>>>();
  ASSERT_EQ(expected, actual);
}

TEST(json, test_json_schema_sized_string_min4_max6) {
  constexpr auto expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","allOf":[{"type":"string","minLength":4},{"type":"string","maxLength":6}],"definitions":{}})";

  auto actual = to_schema<SizedMinMax<std::string, 4, 6>>();
  ASSERT_EQ(expected, actual);
  actual = to_schema<SizedAllOf<std::string, Minimum<4>, Maximum<6>>>();
  ASSERT_EQ(expected, actual);
}

TEST(json, test_json_schema_sized_vector_anyof_eq3_eq7) {
  constexpr auto expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","anyOf":[{"type":"array","items":{"type":"integer"},"minItems":3,"maxItems":3},{"type":"array","items":{"type":"integer"},"minItems":7,"maxItems":7}],"definitions":{}})";

  auto actual =
      to_schema<SizedAnyOf<std::vector<int>, EqualTo<3>, EqualTo<7>>>();
  ASSERT_EQ(expected, actual);
}

TEST(json, test_json_schema_sized_set_anyof_eq15_eq16) {
  constexpr auto expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","anyOf":[{"type":"array","items":{"type":"integer"},"minItems":16,"maxItems":16},{"type":"array","items":{"type":"integer"},"minItems":16,"maxItems":16}],"definitions":{}})";

  auto actual =
      to_schema<SizedAnyOf<std::vector<int>, EqualTo<16>, EqualTo<16>>>();
  ASSERT_EQ(expected, actual);
}

TEST(json, test_json_schema_sized_string_anyof_eq1_eq10) {
  constexpr auto expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","anyOf":[{"type":"array","items":{"type":"integer"},"minItems":1,"maxItems":1},{"type":"array","items":{"type":"integer"},"minItems":10,"maxItems":10}],"definitions":{}})";

  auto actual =
      to_schema<SizedAnyOf<std::vector<int>, EqualTo<1>, EqualTo<10>>>();
  ASSERT_EQ(expected, actual);
}
}  // namespace test_json_schema_size_validation
