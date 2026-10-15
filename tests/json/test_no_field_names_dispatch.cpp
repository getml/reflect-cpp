#include <rfl/json.hpp>

#include "../check_no_field_names_dispatch.hpp"
namespace test_no_field_names_dispatch {
TEST(json, no_field_names_dispatch_roundtrip) {
  check_roundtrip(
      [](const auto& value) {
        return rfl::json::write<rfl::NoFieldNames>(value);
      },
      []<class T>(const auto& bytes) {
        return rfl::json::read<T, rfl::NoFieldNames>(bytes);
      });
}
}  // namespace test_no_field_names_dispatch

#include "../check_positional_field_dispatch.hpp"

namespace test_no_field_names_dispatch {
TEST(json, no_field_names_dispatch_errors_and_cleanup) {
  test_positional_field_dispatch::check_errors(
      [](const auto& value) {
        return rfl::json::write<rfl::NoFieldNames>(value);
      },
      []<class T>(const auto& bytes) {
        return rfl::json::read<T, rfl::NoFieldNames>(bytes);
      });
}

TEST(json, no_field_names_dispatch_missing_and_extra_fields) {
  const auto missing =
      rfl::json::read<Record, rfl::NoFieldNames>(R"([1,"middle"])");
  ASSERT_FALSE(missing);
  EXPECT_NE(
      std::string(missing.error().what()).find("Field named 'last' not found"),
      std::string::npos);
  const auto defaults =
      rfl::json::read<Record, rfl::NoFieldNames, rfl::DefaultIfMissing>(
          R"([1,"middle"])");
  ASSERT_TRUE(defaults);
  EXPECT_EQ(defaults->last, 0);
  const auto extra =
      rfl::json::read<Record, rfl::NoFieldNames>(R"([1,"middle",2,3])");
  ASSERT_FALSE(extra);
  EXPECT_NE(
      std::string(extra.error().what()).find("Expected a maximum of 3 fields"),
      std::string::npos);
  const auto extra_defaults =
      rfl::json::read<Record, rfl::NoFieldNames, rfl::DefaultIfMissing>(
          R"([1,"middle",2,3])");
  ASSERT_FALSE(extra_defaults);
  EXPECT_EQ(extra_defaults.error().what(), extra.error().what());
}

struct Defaults {
  rfl::Validator<int, rfl::Minimum<0>> first = 0;
  std::string middle = "default";
  rfl::Validator<int, rfl::Minimum<0>> last = 0;
};
TEST(json, no_field_names_dispatch_default_errors) {
  const auto result =
      rfl::json::read<Defaults, rfl::NoFieldNames, rfl::DefaultIfMissing>(
          R"([-1,"middle",-2])");
  ASSERT_FALSE(result);
  const std::string message = result.error().what();
  EXPECT_NE(message.find("Found 2 errors:"), std::string::npos);
  EXPECT_LT(message.find("Failed to parse field 'first'"),
            message.find("Failed to parse field 'last'"));
}

struct Empty {};
TEST(json, no_field_names_dispatch_empty) {
  EXPECT_TRUE((rfl::json::read<Empty, rfl::NoFieldNames>("[]")));
  EXPECT_FALSE((rfl::json::read<Empty, rfl::NoFieldNames>("[1]")));
  EXPECT_TRUE(
      (rfl::json::read<Empty, rfl::NoFieldNames, rfl::DefaultIfMissing>("[]")));
  EXPECT_FALSE((
      rfl::json::read<Empty, rfl::NoFieldNames, rfl::DefaultIfMissing>("[1]")));
}
}  // namespace test_no_field_names_dispatch
