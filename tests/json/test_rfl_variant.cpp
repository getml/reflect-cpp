#include <cassert>
#include <string>

#include "write_and_read.hpp"
#include <memory>

import rfl;
import rfl.json;


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
