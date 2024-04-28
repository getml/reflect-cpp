#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_enum1 {

enum class Color { red, green, blue, yellow };

struct Circle {
  float radius;
  Color color;
};

TEST(json, test_enum1) {
  const auto circle = Circle{.radius = 2.0, .color = Color::green};

  write_and_read(circle, R"({"radius":2.0,"color":"green"})");
}

}  // namespace test_enum1
