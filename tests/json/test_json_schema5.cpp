#include <iostream>
#include <optional>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <tuple>
#include <vector>

#include "write_and_read.hpp"

namespace test_json_schema5 {

using Age = rfl::Validator<std::optional<unsigned int>, rfl::Minimum<0>,
                           rfl::Maximum<130>>;

struct Person {
  rfl::Description<"Given name of this person", std::string> first_name;
  rfl::Description<"Optional family name of this person",
                   std::optional<std::string>>
      last_name;
  Age age;
};

TEST(json, test_json_schema5) {
  const auto json_schema = rfl::json::to_schema<Person>();

  const std::string expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/definitions/test_json_schema5__Person","definitions":{"test_json_schema5__Person":{"type":"object","properties":{"first_name":{"type":"string","description":"Given name of this person"},"last_name":{"description":"Optional family name of this person","anyOf":[{"type":"string"},{"type":"null"}]},"age":{"allOf":[{"minimum":0,"type":"number"},{"maximum":130,"type":"number"}]}},"required":["first_name"]}}})";

  EXPECT_EQ(json_schema, expected);
}
}  // namespace test_json_schema5
