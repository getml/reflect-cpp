#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_rfl_variant {

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

TEST(json, test_rfl_variant) {
  const Shapes r = Rectangle{.height = 10, .width = 5};

  write_and_read(r, R"({"height":10.0,"width":5.0})");
}
}  // namespace test_rfl_variant
