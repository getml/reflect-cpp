#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_enum6 {

enum class Color { red, green, blue, yellow };

struct Circle {
  float radius;
  Color color;
};

TEST(json, test_enum6) {
  auto mutable_circle = Circle{.radius = 2.0, .color = Color::red};

  rfl::get_underlying_enumerators<Color>().apply([&](const auto& field) {
    if (field.name() == "blue") {
      mutable_circle.color = static_cast<Color>(field.value());
    }
  });

  write_and_read(mutable_circle, R"({"radius":2.0,"color":"blue"})");
}

}  // namespace test_enum6
