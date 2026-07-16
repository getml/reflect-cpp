#include <optional>
#include <string>
import rfl;
import rfl.json;


namespace test_json_schema5 {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  rfl::Description<"Given name of this person", std::string> first_name;
  rfl::Description<"Optional family name of this person",
                   std::optional<std::string>>
      last_name;
  Age age;
  rfl::DefaultVal<std::string> nickname = "peter";
};

TEST(json, test_json_schema5) {
  const auto json_schema = rfl::json::to_schema<Person>();

  const std::string expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/$defs/test_json_schema5__Person","$defs":{"test_json_schema5__Person":{"type":"object","properties":{"first_name":{"type":"string","description":"Given name of this person"},"last_name":{"description":"Optional family name of this person","anyOf":[{"type":"string"},{"type":"null"}]},"age":{"allOf":[{"minimum":0,"type":"integer"},{"maximum":130,"type":"integer"}]},"nickname":{"type":"string","default":"peter"}},"required":["first_name","age"]}}})";

  EXPECT_EQ(json_schema, expected);
}
}  // namespace test_json_schema5
