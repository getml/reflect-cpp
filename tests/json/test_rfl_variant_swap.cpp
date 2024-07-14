#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_rfl_variant_swap {

struct Circle {
  double radius;
};

struct Rectangle {
  double height;
  double width;
};

struct Square {
  double width;
};

using Shapes = rfl::Variant<Circle, Rectangle, std::unique_ptr<Square>>;

TEST(json, test_rfl_variant_swap) {
  Shapes r = Rectangle{.height = 10, .width = 5};
  Shapes c = Circle{.radius = 3};
  r.swap(c);
  write_and_read(c, R"({"height":10.0,"width":5.0})");
  write_and_read(r, R"({"radius":3.0})");
}
}  // namespace test_rfl_variant_swap
