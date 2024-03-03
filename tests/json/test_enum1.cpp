#include "test_enum1.hpp"

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

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const auto circle = Circle{.radius = 2.0, .color = Color::green};

  write_and_read(circle, R"({"radius":2.0,"color":"green"})");
}

}  // namespace test_enum1
