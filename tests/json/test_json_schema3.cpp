#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "write_and_read.hpp"

namespace test_json_schema3 {

enum class Color { red, green, blue };

struct Rectangle {
  double width;
  double height;
};

struct Square {
  double width;
};

struct Circle {
  double radius;
};

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

using FieldVariant =
    rfl::Variant<rfl::Field<"rectangle", Rectangle>,
                 rfl::Field<"square", Square>, rfl::Field<"circle", Circle>>;

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  rfl::Description<"Must be a proper email in the form xxx@xxx.xxx.",
                   rfl::Email>
      email;
  std::string town = "Springfield";
  Color color;
  Age age;
  float salary;
  rfl::Description<
      "The person's children. Pass an empty array for no children.",
      std::vector<Person>>
      children;
  std::variant<Color, std::vector<Person>, int> variant;
  std::tuple<Color, std::vector<Person>, int> tuple;
  rfl::Rename<"taggedUnion",
              rfl::TaggedUnion<"shape", Rectangle, Square, Circle>>
      tagged_union;
  rfl::Rename<"fieldVariant", FieldVariant> field_variant;
};

TEST(json, test_json_schema3) {
  using DescribedType = rfl::Description<
      "JSON schema that describes the required "
      "attributes for the person class.",
      Person>;

  const auto json_schema =
      rfl::json::to_schema<DescribedType, rfl::NoFieldNames>();

  const std::string expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/definitions/test_json_schema3__Person","description":"JSON schema that describes the required attributes for the person class.","definitions":{"test_json_schema3__Circle":{"type":"array","prefixItems":[{"type":"number"}],"items":false},"test_json_schema3__Circle__tagged":{"type":"array","prefixItems":[{"type":"string","enum":["Circle"]},{"type":"number"}],"items":false},"test_json_schema3__Person":{"type":"array","prefixItems":[{"type":"string"},{"type":"string"},{"type":"string","description":"Must be a proper email in the form xxx@xxx.xxx.","pattern":"^[a-zA-Z0-9._%+\\-]+@[a-zA-Z0-9.\\-]+\\.[a-zA-Z]{2,}$"},{"type":"string"},{"type":"string","enum":["red","green","blue"]},{"allOf":[{"minimum":0,"type":"integer"},{"maximum":130,"type":"integer"}]},{"type":"number"},{"type":"array","description":"The person's children. Pass an empty array for no children.","items":{"$ref":"#/definitions/test_json_schema3__Person"}},{"anyOf":[{"type":"string","enum":["red","green","blue"]},{"type":"array","items":{"$ref":"#/definitions/test_json_schema3__Person"}},{"type":"integer"}]},{"type":"array","prefixItems":[{"type":"string","enum":["red","green","blue"]},{"type":"array","items":{"$ref":"#/definitions/test_json_schema3__Person"}},{"type":"integer"}],"items":false},{"anyOf":[{"$ref":"#/definitions/test_json_schema3__Rectangle__tagged"},{"$ref":"#/definitions/test_json_schema3__Square__tagged"},{"$ref":"#/definitions/test_json_schema3__Circle__tagged"}]},{"anyOf":[{"type":"array","prefixItems":[{"$ref":"#/definitions/test_json_schema3__Rectangle"}],"items":false},{"type":"array","prefixItems":[{"$ref":"#/definitions/test_json_schema3__Square"}],"items":false},{"type":"array","prefixItems":[{"$ref":"#/definitions/test_json_schema3__Circle"}],"items":false}]}],"items":false},"test_json_schema3__Rectangle":{"type":"array","prefixItems":[{"type":"number"},{"type":"number"}],"items":false},"test_json_schema3__Rectangle__tagged":{"type":"array","prefixItems":[{"type":"string","enum":["Rectangle"]},{"type":"number"},{"type":"number"}],"items":false},"test_json_schema3__Square":{"type":"array","prefixItems":[{"type":"number"}],"items":false},"test_json_schema3__Square__tagged":{"type":"array","prefixItems":[{"type":"string","enum":["Square"]},{"type":"number"}],"items":false}}})";

  EXPECT_EQ(json_schema, expected);
}
}  // namespace test_json_schema3
