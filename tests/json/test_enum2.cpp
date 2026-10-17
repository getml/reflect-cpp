#include <cassert>
#include <string>

#include "write_and_read.hpp"
#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


namespace test_enum2 {

enum class Color { red, green, blue, yellow };

struct Circle {
  float radius;
  Color color;
};

TEST(json, test_enum2) {
  auto mutable_circle = Circle{.radius = 2.0, .color = Color::green};

  if (auto color = rfl::string_to_enum<Color>("red"); color) {
    mutable_circle.color = *color;
  }
  write_and_read(mutable_circle, R"({"radius":2.0,"color":"red"})");
}

}  // namespace test_enum2
