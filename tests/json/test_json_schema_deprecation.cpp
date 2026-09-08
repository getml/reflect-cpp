#include <optional>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_deprecated_schema {

struct DeprecatedBasicField {
  rfl::Deprecated<"Use 'new_name' instead.", "The old name", std::string>
      old_name;
  rfl::Description<"The new name", std::string> new_name;
};

TEST(json, test_deprecated_basic_field) {
  const auto json_schema = rfl::json::to_schema<DeprecatedBasicField>();

  const std::string expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/$defs/test_deprecated_schema__DeprecatedBasicField","$defs":{"test_deprecated_schema__DeprecatedBasicField":{"type":"object","properties":{"old_name":{"type":"string","description":"The old name","deprecated":true,"deprecationMessage":"Use 'new_name' instead."},"new_name":{"type":"string","description":"The new name"}},"required":["old_name","new_name"]}}})";

  EXPECT_EQ(json_schema, expected);
}

struct DeprecatedArrayField {
  rfl::Deprecated<"Use 'items' instead.", "Legacy item list",
                  std::vector<std::string>>
      old_items;
  rfl::Description<"Current item list", std::vector<std::string>> items;
};

TEST(json, test_deprecated_array_field) {
  const auto json_schema = rfl::json::to_schema<DeprecatedArrayField>();

  const std::string expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/$defs/test_deprecated_schema__DeprecatedArrayField","$defs":{"test_deprecated_schema__DeprecatedArrayField":{"type":"object","properties":{"old_items":{"type":"array","description":"Legacy item list","deprecated":true,"deprecationMessage":"Use 'items' instead.","items":{"type":"string"}},"items":{"type":"array","description":"Current item list","items":{"type":"string"}}},"required":["old_items","items"]}}})";

  EXPECT_EQ(json_schema, expected);
}

struct DeprecatedOptionalField {
  rfl::Deprecated<"Removed in v2.", "Old optional setting",
                  std::optional<int>>
      old_setting;
};

TEST(json, test_deprecated_optional_field) {
  const auto json_schema = rfl::json::to_schema<DeprecatedOptionalField>();

  const std::string expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/$defs/test_deprecated_schema__DeprecatedOptionalField","$defs":{"test_deprecated_schema__DeprecatedOptionalField":{"type":"object","properties":{"old_setting":{"description":"Old optional setting","deprecated":true,"deprecationMessage":"Removed in v2.","anyOf":[{"type":"integer"},{"type":"null"}]}},"required":[]}}})";

  EXPECT_EQ(json_schema, expected);
}

TEST(json, test_deprecated_read_write) {
  const auto expected = R"({"old_name":"legacy","new_name":"current"})";
  const auto obj =
      DeprecatedBasicField{.old_name = "legacy", .new_name = "current"};
  write_and_read(obj, expected);
}

}  // namespace test_deprecated_schema
