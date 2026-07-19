import rfl;
import rfl.json;


namespace test_json_schema7 {

struct Circle {
  double radius;
};
struct Rectangle {
  double height;
  double width;
};

using Shapes = rfl::Variant<Circle, Rectangle>;

TEST(json, test_json_schema7) {
  const Shapes r = Rectangle{.height = 10, .width = 5};

  const auto json_schema =
      rfl::json::to_schema<Shapes, rfl::AddTagsToVariants>();

  ASSERT_EQ(
      json_schema,
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","anyOf":[{"type":"object","properties":{"Circle":{"$ref":"#/$defs/test_json_schema7__Circle"}},"required":["Circle"]},{"type":"object","properties":{"Rectangle":{"$ref":"#/$defs/test_json_schema7__Rectangle"}},"required":["Rectangle"]}],"$defs":{"test_json_schema7__Circle":{"type":"object","properties":{"radius":{"type":"number"}},"required":["radius"]},"test_json_schema7__Rectangle":{"type":"object","properties":{"height":{"type":"number"},"width":{"type":"number"}},"required":["height","width"]}}})");
}
}  // namespace test_json_schema7
