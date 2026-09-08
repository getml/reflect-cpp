#include <cassert>
#include <rfl.hpp>
#include <rfl/json.hpp>

#include <gtest/gtest.h>

namespace test_enum_error_messages {

enum class Color { red, green, blue, yellow };

TEST(json, test_enum_error_message_invalid_enum) {

  const auto result = rfl::string_to_enum<Color>("bart");

  const std::string expected = R"(Invalid enum value: 'bart'. Must be one of [red, green, blue, yellow].)";

  EXPECT_TRUE(!result.has_value());

  EXPECT_EQ(result.error().what(), expected);
}

TEST(json, test_enum_error_message_empty_enum) {

  const auto result = rfl::string_to_enum<Color>("");

  const std::string expected = R"(Invalid enum value: ''. Must be one of [red, green, blue, yellow].)";

  EXPECT_TRUE(!result.has_value());

  EXPECT_EQ(result.error().what(), expected);
}

TEST(json, test_enum_error_message_case_sensitive) {

  const auto result = rfl::string_to_enum<Color>("RED");

  const std::string expected = R"(Invalid enum value: 'RED'. Must be one of [red, green, blue, yellow].)";

  EXPECT_TRUE(!result.has_value());

  EXPECT_EQ(result.error().what(), expected);
}


}  // namespace test_enum_error_messages
