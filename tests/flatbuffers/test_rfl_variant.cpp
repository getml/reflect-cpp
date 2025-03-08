#include <cassert>
#include <iostream>
#include <rfl.hpp>
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

struct Shapes {
  rfl::Variant<Circle, Rectangle, std::unique_ptr<Square>> root;
};

TEST(flatbuffers, test_rfl_variant) {
  const auto r = Shapes{Rectangle{.height = 10, .width = 5}};

  write_and_read(r);
}
}  // namespace test_variant
