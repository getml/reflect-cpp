#include <gtest/gtest.h>

#include <ctime>
#include <string>

#include <rfl.hpp>
#include <rfl/json.hpp>

// transform_camel_case prepends '_' to names starting with uppercase
// File: include/rfl/internal/transform_case.hpp:60-73
// When i==0 and the first character is uppercase, it unconditionally prepends
// '_'. E.g. "MyField" → "_my_field" instead of "my_field".
namespace test_camel_case_leading_underscore {

struct CamelStruct {
  int MyField = 42;
};

TEST(regression, camel_case_to_snake_case_no_leading_underscore) {
  const auto s = CamelStruct{.MyField = 42};
  const auto json = rfl::json::write<rfl::CamelCaseToSnakeCase>(s);
  // Should produce "my_field", not "_my_field"
  EXPECT_EQ(json, R"({"my_field":42})")
      << "CamelCaseToSnakeCase should not prepend underscore. Got: " << json;
}

}  // namespace test_camel_case_leading_underscore

// Timestamp::to_time_t() double-subtracts timezone
// File: include/rfl/Timestamp.hpp:120
// `timegm(&tm) - tm_.tm_gmtoff` — timegm already treats input as UTC,
// so subtracting tm_gmtoff is a double correction. When tm_gmtoff != 0,
// the result is wrong.
namespace test_timestamp_double_timezone {

#if !defined(_MSC_VER) && !defined(__MINGW32__)
TEST(regression, timestamp_to_time_t_no_double_timezone_correction) {
  using TS = rfl::Timestamp<"%Y-%m-%d %H:%M:%S">;
  // Known ground truth: 2024-01-15 12:00:00 UTC = 1705320000
  constexpr time_t ground_truth = 1705320000;

  auto ts = TS("2024-01-15 12:00:00");
  // Simulate strptime leaving a non-zero tm_gmtoff
  // (which can happen depending on libc implementation)
  ts.tm().tm_gmtoff = 3600;  // +1 hour

  const auto actual = ts.to_time_t();
  // timegm already treats input as UTC. Subtracting tm_gmtoff is wrong —
  // it causes a double timezone correction.
  EXPECT_EQ(actual, ground_truth)
      << "to_time_t() should return " << ground_truth
      << " for 2024-01-15 12:00:00 UTC regardless of tm_gmtoff, "
         "but got " << actual
      << " (difference: " << (ground_truth - actual) << "s)";
}
#endif

}  // namespace test_timestamp_double_timezone

// FieldVariantParser wrong error message for zero fields
// File: include/rfl/parsing/FieldVariantParser.hpp:47-50
// When the input object has zero matching fields, the error says
// "found more than one" which is misleading. Should say something
// like "found none".
namespace test_field_variant_empty_object {

using FieldVar =
    rfl::Variant<rfl::Field<"a", int>, rfl::Field<"b", int>>;

TEST(regression, field_variant_empty_object_error_message) {
  const auto result = rfl::json::read<FieldVar>("{}");
  ASSERT_FALSE(result) << "Parsing empty object as FieldVariant should fail";
  const std::string msg = result.error().what();
  EXPECT_EQ(msg.find("more than one"), std::string::npos)
      << "Error for zero matching fields should NOT say 'more than one'. "
         "Got: " << msg;
}

}  // namespace test_field_variant_empty_object

// Parser_string_view / Parser_span memory leak bugs are tested
// in tests/alloc/ — a separate executable with custom operator new[]/delete[]
// that tracks allocations to detect leaks without ASAN.
