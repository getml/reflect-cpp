#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

namespace test_enum_descriptions {

// Define an enum with descriptions
enum class Color { red, green, blue };

// An enum without descriptions for comparison
enum class Size { small, medium, large };

struct Config {
  Color color;
  Size size;
};

}  // namespace test_enum_descriptions

// Specialize enum_descriptions to provide descriptions for Color values
template <>
struct rfl::config::enum_descriptions<test_enum_descriptions::Color> {
  static constexpr bool has_descriptions = true;
  static constexpr std::string_view get(test_enum_descriptions::Color value) {
    switch (value) {
      case test_enum_descriptions::Color::red:
        return "The color red";
      case test_enum_descriptions::Color::green:
        return "The color green";
      case test_enum_descriptions::Color::blue:
        return "The color blue";
      default:
        return "";
    }
  }
};

namespace test_enum_descriptions {

TEST(json, test_enum_descriptions_schema) {
  const auto json_schema = rfl::json::to_schema<Config>();

  // The schema should contain oneOf with const/description for Color
  EXPECT_TRUE(json_schema.find("\"oneOf\"") != std::string::npos)
      << "Expected oneOf for described enum. Schema: " << json_schema;
  EXPECT_TRUE(json_schema.find("\"const\":\"red\"") != std::string::npos)
      << "Expected const for red. Schema: " << json_schema;
  EXPECT_TRUE(json_schema.find("\"description\":\"The color red\"") !=
              std::string::npos)
      << "Expected description for red. Schema: " << json_schema;
  EXPECT_TRUE(json_schema.find("\"const\":\"green\"") != std::string::npos)
      << "Expected const for green. Schema: " << json_schema;
  EXPECT_TRUE(json_schema.find("\"const\":\"blue\"") != std::string::npos)
      << "Expected const for blue. Schema: " << json_schema;

  // Size should still use regular enum format
  EXPECT_TRUE(json_schema.find("\"enum\":[\"small\",\"medium\",\"large\"]") !=
              std::string::npos)
      << "Expected regular enum for Size. Schema: " << json_schema;
}

TEST(json, test_enum_descriptions_read_write) {
  // Verify that read/write still works correctly with described enums
  const Config config{.color = Color::green, .size = Size::medium};

  const auto json_string = rfl::json::write(config);
  EXPECT_EQ(json_string, R"({"color":"green","size":"medium"})");

  const auto parsed = rfl::json::read<Config>(json_string);
  EXPECT_TRUE(parsed.has_value()) << "Failed to parse: " << parsed.error().what();
  EXPECT_EQ(parsed.value().color, Color::green);
  EXPECT_EQ(parsed.value().size, Size::medium);
}

}  // namespace test_enum_descriptions
