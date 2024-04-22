#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_enum5 {

enum class Color { red, green, blue, yellow };

struct Circle {
  float radius;
  Color color;
};

TEST(json, test_enum5) {
  auto mutable_circle = Circle{.radius = 2.0, .color = Color::blue};

  rfl::get_enumerators<Color>().apply([&](auto field) {
    if constexpr (decltype(field)::name() == "green") {
      mutable_circle.color = field.value();
    }
  });
  write_and_read(mutable_circle, R"({"radius":2.0,"color":"green"})");
}

}  // namespace test_enum5
