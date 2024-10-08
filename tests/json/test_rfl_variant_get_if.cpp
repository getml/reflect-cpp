#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_rfl_variant_get_if {

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

TEST(json, test_rfl_variant_get_if) {
  const Shapes r = Rectangle{.height = 10, .width = 5};
  EXPECT_TRUE(rfl::get_if<1>(&r) && true);
  EXPECT_FALSE(rfl::get_if<0>(&r) && true);
}
}  // namespace test_rfl_variant_get_if
