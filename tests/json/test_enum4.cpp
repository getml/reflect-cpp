#include "test_enum4.hpp"

#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_enum4 {

enum class Color { red, green, blue, yellow };

struct Circle {
  float radius;
  Color color;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  auto mutable_circle = Circle{.radius = 2.0, .color = Color::red};

  if (auto color = rfl::string_to_enum<Color>(rfl::enum_to_string(Color::blue));
      color) {
    mutable_circle.color = *color;
  }

  write_and_read(mutable_circle, R"({"radius":2.0,"color":"blue"})");
}

}  // namespace test_enum4
