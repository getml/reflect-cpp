#include <cassert>

#include "write_and_read.hpp"
#include <memory>

import rfl.avro;
import rfl;


namespace test_variant {

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

TEST(avro, test_rfl_variant) {
  const Shapes r = Rectangle{.height = 10, .width = 5};

  write_and_read_with_json(r);
}
}  // namespace test_variant
