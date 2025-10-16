#include <cassert>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_reflector_schema {

struct Person {
  std::string first_name = "Homer";
  std::string last_name = "Simpson";
};

struct Parent : Person {
 public:
  std::vector<Person> children;
};

}  // namespace test_reflector_schema

namespace rfl {
template <>
struct Reflector<test_reflector_schema::Parent> {
  struct ReflType {
    std::string first_name;
    std::string last_name;
    std::vector<test_reflector_schema::Person> children;
  };

  static ReflType from(const test_reflector_schema::Parent& v) {
    return {v.first_name, v.last_name, v.children};
  }
};
}  // namespace rfl

namespace test_reflector_schema {

TEST(json, test_reflector_schema) {
  const std::string expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/$defs/test_reflector_schema__Parent","$defs":{"rfl__Reflector_test_reflector_schema__Parent___ReflType":{"type":"object","properties":{"first_name":{"type":"string"},"last_name":{"type":"string"},"children":{"type":"array","items":{"$ref":"#/$defs/test_reflector_schema__Person"}}},"required":["first_name","last_name","children"]},"test_reflector_schema__Parent":{"$ref":"#/$defs/rfl__Reflector_test_reflector_schema__Parent___ReflType"},"test_reflector_schema__Person":{"type":"object","properties":{"first_name":{"type":"string"},"last_name":{"type":"string"}},"required":["first_name","last_name"]}}})";

  EXPECT_EQ(rfl::json::to_schema<Parent>(), expected);
}
}  // namespace test_reflector_schema
