#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_enum3 {

enum class Color { red, green, blue, yellow };

struct Circle {
  float radius;
  Color color;
};

TEST(json, test_enum3) {
  auto mutable_circle = Circle{.radius = 2.0, .color = Color::red};

  if (auto color = rfl::string_to_enum<Color>("bart"); color) {
    mutable_circle.color = *color;
  }
  write_and_read(mutable_circle, R"({"radius":2.0,"color":"red"})");
}

}  // namespace test_enum3
