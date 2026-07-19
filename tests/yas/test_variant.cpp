#include <iostream>
#include <string>
#include <vector>

#include "write_and_read.hpp"
#include <variant>

import rfl;
import rfl.yas;


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

using Shapes = std::variant<Circle, Rectangle, Square>;

TEST(yas, test_variant) {
  const Shapes r = Rectangle{.height = 10, .width = 5};

  write_and_read(r);
}

}  // namespace test_variant
