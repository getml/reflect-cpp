#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

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

using Shapes = std::variant<Circle, Rectangle, std::unique_ptr<Square>>;

TEST(xml, test_variant) {
  const Shapes r = Rectangle{.height = 10, .width = 5};

  write_and_read<"root">(r);
}
}  // namespace test_variant
