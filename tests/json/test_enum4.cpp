#include <cassert>
#include <string>

#include "write_and_read.hpp"
import rfl;
import rfl.json;


namespace test_enum4 {

enum class Color { red, green, blue, yellow };

struct Circle {
  float radius;
  Color color;
};

TEST(json, test_enum4) {

  auto mutable_circle = Circle{.radius = 2.0, .color = Color::red};

  if (auto color = rfl::string_to_enum<Color>(rfl::enum_to_string(Color::blue));
      color) {
    mutable_circle.color = *color;
  }

  write_and_read(mutable_circle, R"({"radius":2.0,"color":"blue"})");
}

}  // namespace test_enum4
