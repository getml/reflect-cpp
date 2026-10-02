#include <optional>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

namespace test_json_schema8 {

struct Address {
  std::string street;
  int number;
};

struct Person {
  std::string name;
  Address address;  // Ensures that nested objects also get
                    // `additionalProperties: false`.
  std::optional<int> age;
};

TEST(json, test_json_schema8) {
  const auto json_schema = rfl::json::to_schema<Person, rfl::NoExtraFields>();

  EXPECT_EQ(
      json_schema,
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/$defs/test_json_schema8__Person","$defs":{"test_json_schema8__Address":{"type":"object","properties":{"street":{"type":"string"},"number":{"type":"integer"}},"required":["street","number"],"additionalProperties":false},"test_json_schema8__Person":{"type":"object","properties":{"name":{"type":"string"},"address":{"$ref":"#/$defs/test_json_schema8__Address"},"age":{"anyOf":[{"type":"integer"},{"type":"null"}]}},"required":["name","address"],"additionalProperties":false}}})");
}

TEST(json, test_json_schema8_without_no_extra_fields) {
  const auto json_schema = rfl::json::to_schema<Person>();

  EXPECT_EQ(
      json_schema,
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/$defs/test_json_schema8__Person","$defs":{"test_json_schema8__Address":{"type":"object","properties":{"street":{"type":"string"},"number":{"type":"integer"}},"required":["street","number"]},"test_json_schema8__Person":{"type":"object","properties":{"name":{"type":"string"},"address":{"$ref":"#/$defs/test_json_schema8__Address"},"age":{"anyOf":[{"type":"integer"},{"type":"null"}]}},"required":["name","address"]}}})");
}
}  // namespace test_json_schema8
