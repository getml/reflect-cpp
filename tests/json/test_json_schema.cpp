#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "write_and_read.hpp"

namespace test_json_schema {

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
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
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

TEST(json, test_json_schema) {
  const auto json_schema = rfl::json::to_schema<
      rfl::Description<"JSON schema that describes the required "
                       "attributes for the person class.",
                       Person>>();

  const std::string expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/definitions/test_json_schema__Person","description":"JSON schema that describes the required attributes for the person class.","definitions":{"test_json_schema__Circle":{"type":"object","properties":{"radius":{"type":"number"}},"required":["radius"]},"test_json_schema__Circle__tagged":{"type":"object","properties":{"radius":{"type":"number"},"shape":{"type":"string","enum":["Circle"]}},"required":["radius","shape"]},"test_json_schema__Person":{"type":"object","properties":{"age":{"allOf":[{"minimum":0,"type":"integer"},{"maximum":130,"type":"integer"}]},"children":{"type":"array","description":"The person's children. Pass an empty array for no children.","items":{"$ref":"#/definitions/test_json_schema__Person"}},"color":{"type":"string","enum":["red","green","blue"]},"email":{"type":"string","description":"Must be a proper email in the form xxx@xxx.xxx.","pattern":"^[a-zA-Z0-9._%+\\-]+@[a-zA-Z0-9.\\-]+\\.[a-zA-Z]{2,}$"},"fieldVariant":{"anyOf":[{"type":"object","properties":{"rectangle":{"$ref":"#/definitions/test_json_schema__Rectangle"}},"required":["rectangle"]},{"type":"object","properties":{"square":{"$ref":"#/definitions/test_json_schema__Square"}},"required":["square"]},{"type":"object","properties":{"circle":{"$ref":"#/definitions/test_json_schema__Circle"}},"required":["circle"]}]},"firstName":{"type":"string"},"lastName":{"type":"string"},"salary":{"type":"number"},"taggedUnion":{"anyOf":[{"$ref":"#/definitions/test_json_schema__Rectangle__tagged"},{"$ref":"#/definitions/test_json_schema__Square__tagged"},{"$ref":"#/definitions/test_json_schema__Circle__tagged"}]},"town":{"type":"string"},"tuple":{"type":"array","prefixItems":[{"type":"string","enum":["red","green","blue"]},{"type":"array","items":{"$ref":"#/definitions/test_json_schema__Person"}},{"type":"integer"}],"items":false},"variant":{"anyOf":[{"type":"string","enum":["red","green","blue"]},{"type":"array","items":{"$ref":"#/definitions/test_json_schema__Person"}},{"type":"integer"}]}},"required":["age","children","color","email","fieldVariant","firstName","lastName","salary","taggedUnion","town","tuple","variant"]},"test_json_schema__Rectangle":{"type":"object","properties":{"height":{"type":"number"},"width":{"type":"number"}},"required":["height","width"]},"test_json_schema__Rectangle__tagged":{"type":"object","properties":{"height":{"type":"number"},"shape":{"type":"string","enum":["Rectangle"]},"width":{"type":"number"}},"required":["height","shape","width"]},"test_json_schema__Square":{"type":"object","properties":{"width":{"type":"number"}},"required":["width"]},"test_json_schema__Square__tagged":{"type":"object","properties":{"shape":{"type":"string","enum":["Square"]},"width":{"type":"number"}},"required":["shape","width"]}}})";

  EXPECT_EQ(json_schema, expected);
}
}  // namespace test_json_schema
