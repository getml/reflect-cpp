#include <gtest/gtest.h>

#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

namespace test_field_variant_multiple_fields {

struct Circle {
  double radius;
};

struct Rectangle {
  double height;
  double width;
};

using Shapes =
    rfl::Variant<rfl::Field<"circle", Circle>, rfl::Field<"rectangle", Rectangle>>;

TEST(json, test_field_variant_multiple_fields) {
  const std::string faulty_string =
      R"({"circle":{"radius":2.0},"rectangle":{"height":10.0,"width":5.0}})";

  const auto result = rfl::json::read<Shapes>(faulty_string);

  EXPECT_TRUE(!result.has_value() && true);
  EXPECT_EQ(result.error().what(),
            "Could not parse rfl::Variant: Expected the object to have "
            "exactly one field, but found more than one.");
}

TEST(json, test_field_variant_unknown_then_known_field) {
  const std::string faulty_string =
      R"({"triangle":{"base":3.0},"circle":{"radius":2.0}})";

  const auto result = rfl::json::read<Shapes>(faulty_string);

  EXPECT_TRUE(!result.has_value() && true);
}

}  // namespace test_field_variant_multiple_fields
