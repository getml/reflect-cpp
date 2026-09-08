#include <gtest/gtest.h>

#include <cstdint>
#include <rfl.hpp>
#include <rfl/json.hpp>

namespace test_enum_names_only {

enum class Color { red = 1, green = 2, blue = 3 };

enum class Perm {
  read = 1,
  write = 2,
  exec = 4
};

inline Perm operator|(Perm a, Perm b) noexcept {
  return static_cast<Perm>(static_cast<int>(a) | static_cast<int>(b));
}

// An enum with a narrow underlying type.
enum class Narrow : uint8_t { low = 1, high = 2 };

struct Circle {
  float radius;
  Color color;
};

struct File {
  int id;
  Perm perm;
};

TEST(json, test_numeric_enum_allowed_by_default) {
  // Legacy behavior: numeric values are accepted as cast values, even when
  // they do not correspond to a declared enumerator.
  const auto res = rfl::json::read<Circle>(R"({"radius":2.0,"color":"4"})");
  ASSERT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  EXPECT_EQ(static_cast<int>(res.value().color), 4);
}

TEST(json, test_enum_name_allowed_by_default) {
  const auto res = rfl::json::read<Circle>(R"({"radius":2.0,"color":"red"})");
  ASSERT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  EXPECT_EQ(res.value().color, Color::red);
}

TEST(json, test_unknown_enum_name_rejected_by_default) {
  const auto res = rfl::json::read<Circle>(R"({"radius":2.0,"color":"bart"})");
  EXPECT_TRUE(!res.has_value());
  EXPECT_EQ(res.error().what(),
            R"(Failed to parse field 'color': Invalid enum value: 'bart'. Must be one of [red, green, blue].)");
}

TEST(json, test_flag_enum_numeric_allowed_by_default) {
  const auto res = rfl::json::read<File>(R"({"id":1,"perm":"read|8"})");
  ASSERT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  EXPECT_EQ(static_cast<int>(res.value().perm), 9);
}

TEST(json, test_enum_name_allowed_with_processor) {
  const auto res =
      rfl::json::read<Circle, rfl::EnumNamesOnly>(
          R"({"radius":2.0,"color":"red"})");
  ASSERT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  EXPECT_EQ(res.value().color, Color::red);
}

TEST(json, test_declared_numeric_value_rejected_with_processor) {
  // Numeric values are rejected even when they correspond to a declared
  // enumerator: only the declared enumerator names are accepted.
  const auto res =
      rfl::json::read<Circle, rfl::EnumNamesOnly>(
          R"({"radius":2.0,"color":"2"})");
  EXPECT_TRUE(!res.has_value());
  EXPECT_EQ(res.error().what(),
            R"(Failed to parse field 'color': Invalid enum value: '2'. Must be one of [red, green, blue].)");
}

TEST(json, test_undeclared_numeric_value_rejected_with_processor) {
  const auto res =
      rfl::json::read<Circle, rfl::EnumNamesOnly>(
          R"({"radius":2.0,"color":"4"})");
  EXPECT_TRUE(!res.has_value());
  EXPECT_EQ(res.error().what(),
            R"(Failed to parse field 'color': Invalid enum value: '4'. Must be one of [red, green, blue].)");
}

TEST(json, test_negative_numeric_value_rejected_with_processor) {
  const auto res =
      rfl::json::read<Circle, rfl::EnumNamesOnly>(
          R"({"radius":2.0,"color":"-1"})");
  EXPECT_TRUE(!res.has_value());
  EXPECT_EQ(res.error().what(),
            R"(Failed to parse field 'color': Invalid enum value: '-1'. Must be one of [red, green, blue].)");
}

TEST(json, test_unknown_enum_name_rejected_with_processor) {
  const auto res =
      rfl::json::read<Circle, rfl::EnumNamesOnly>(
          R"({"radius":2.0,"color":"bart"})");
  EXPECT_TRUE(!res.has_value());
  EXPECT_EQ(res.error().what(),
            R"(Failed to parse field 'color': Invalid enum value: 'bart'. Must be one of [red, green, blue].)");
}

TEST(json, test_flag_enum_names_allowed_with_processor) {
  const auto res =
      rfl::json::read<File, rfl::EnumNamesOnly>(
          R"({"id":1,"perm":"read|write"})");
  ASSERT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  EXPECT_EQ(res.value().perm, Perm::read | Perm::write);
}

TEST(json, test_flag_enum_numeric_token_rejected_with_processor) {
  const auto res =
      rfl::json::read<File, rfl::EnumNamesOnly>(R"({"id":1,"perm":"read|8"})");
  EXPECT_TRUE(!res.has_value());
  EXPECT_EQ(res.error().what(),
            R"(Failed to parse field 'perm': Invalid enum value: '8'. Must be one of [read, write, exec].)");
}

TEST(json, test_string_to_enum_direct) {
  // With enum_names_only = true, only the declared names are accepted.
  const auto name = rfl::string_to_enum<Color, true>("green");
  ASSERT_TRUE(name.has_value());
  EXPECT_EQ(*name, Color::green);
  const auto declared_numeric = rfl::string_to_enum<Color, true>("2");
  EXPECT_TRUE(!declared_numeric.has_value());
  EXPECT_EQ(declared_numeric.error().what(),
            R"(Invalid enum value: '2'. Must be one of [red, green, blue].)");
  const auto undeclared_numeric = rfl::string_to_enum<Color, true>("4");
  EXPECT_TRUE(!undeclared_numeric.has_value());
  // Values that overflow the narrow underlying type are rejected as well.
  const auto narrow = rfl::string_to_enum<Narrow, true>("300");
  EXPECT_TRUE(!narrow.has_value());
  // The default (permissive) behavior is unchanged: numeric values are
  // accepted as cast values.
  const auto permissive = rfl::string_to_enum<Color>("4");
  ASSERT_TRUE(permissive.has_value());
  EXPECT_EQ(static_cast<int>(*permissive), 4);
  const auto permissive_narrow = rfl::string_to_enum<Narrow>("300");
  ASSERT_TRUE(permissive_narrow.has_value());
  EXPECT_EQ(static_cast<int>(*permissive_narrow), 44);
}

}  // namespace test_enum_names_only
