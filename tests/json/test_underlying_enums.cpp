#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_underlying_enums {

enum class Color { red, green, blue, yellow };

struct Circle {
  float radius;
  Color color;
};

TEST(json, test_underlying_enums) {
  const auto circle = Circle{.radius = 2.0, .color = Color::green};

  write_and_read<rfl::UnderlyingEnums>(circle, R"({"radius":2.0,"color":1})");
}

}  // namespace test_enum1
