#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "write_and_read.hpp"

namespace test_json_schema4 {

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

struct WithExtraFields {
  std::string f1;
  std::string f2;
  rfl::ExtraFields<std::string> extra_fields;
};

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
  rfl::Generic generic;
  WithExtraFields with_extra_fields;
};

TEST(json, test_json_schema4) {
  const auto json_schema =
      rfl::json::to_schema<Person, rfl::DefaultIfMissing>();

  const std::string expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/definitions/test_json_schema4__Person","definitions":{"rfl__Generic":{"anyOf":[{"anyOf":[{"type":"boolean"},{"type":"integer"},{"type":"number"},{"type":"string"},{"type":"object","additionalProperties":{"$ref":"#/definitions/rfl__Generic"}},{"type":"array","items":{"$ref":"#/definitions/rfl__Generic"}}]},{"type":"null"}]},"test_json_schema4__Circle":{"type":"object","properties":{"radius":{"type":"number"}},"required":[]},"test_json_schema4__Circle__tagged":{"type":"object","properties":{"shape":{"type":"string","enum":["Circle"]},"radius":{"type":"number"}},"required":[]},"test_json_schema4__Person":{"type":"object","properties":{"firstName":{"type":"string"},"lastName":{"type":"string"},"email":{"type":"string","description":"Must be a proper email in the form xxx@xxx.xxx.","pattern":"^[a-zA-Z0-9._%+\\-]+@[a-zA-Z0-9.\\-]+\\.[a-zA-Z]{2,}$"},"town":{"type":"string"},"color":{"type":"string","enum":["red","green","blue"]},"age":{"allOf":[{"minimum":0,"type":"integer"},{"maximum":130,"type":"integer"}]},"salary":{"type":"number"},"children":{"type":"array","description":"The person's children. Pass an empty array for no children.","items":{"$ref":"#/definitions/test_json_schema4__Person"}},"variant":{"anyOf":[{"type":"string","enum":["red","green","blue"]},{"type":"array","items":{"$ref":"#/definitions/test_json_schema4__Person"}},{"type":"integer"}]},"tuple":{"type":"array","prefixItems":[{"type":"string","enum":["red","green","blue"]},{"type":"array","items":{"$ref":"#/definitions/test_json_schema4__Person"}},{"type":"integer"}],"items":false},"taggedUnion":{"anyOf":[{"$ref":"#/definitions/test_json_schema4__Rectangle__tagged"},{"$ref":"#/definitions/test_json_schema4__Square__tagged"},{"$ref":"#/definitions/test_json_schema4__Circle__tagged"}]},"fieldVariant":{"anyOf":[{"type":"object","properties":{"rectangle":{"$ref":"#/definitions/test_json_schema4__Rectangle"}},"required":[]},{"type":"object","properties":{"square":{"$ref":"#/definitions/test_json_schema4__Square"}},"required":[]},{"type":"object","properties":{"circle":{"$ref":"#/definitions/test_json_schema4__Circle"}},"required":[]}]},"generic":{"$ref":"#/definitions/rfl__Generic"},"with_extra_fields":{"$ref":"#/definitions/test_json_schema4__WithExtraFields"}},"required":[]},"test_json_schema4__Rectangle":{"type":"object","properties":{"width":{"type":"number"},"height":{"type":"number"}},"required":[]},"test_json_schema4__Rectangle__tagged":{"type":"object","properties":{"shape":{"type":"string","enum":["Rectangle"]},"width":{"type":"number"},"height":{"type":"number"}},"required":[]},"test_json_schema4__Square":{"type":"object","properties":{"width":{"type":"number"}},"required":[]},"test_json_schema4__Square__tagged":{"type":"object","properties":{"shape":{"type":"string","enum":["Square"]},"width":{"type":"number"}},"required":[]},"test_json_schema4__WithExtraFields":{"type":"object","properties":{"f1":{"type":"string"},"f2":{"type":"string"}},"required":[],"additionalProperties":{"type":"string"}}}})";

  EXPECT_EQ(json_schema, expected);
}
}  // namespace test_json_schema4
