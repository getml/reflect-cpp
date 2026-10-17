#include <cassert>

#include <memory>

#include "write_and_read.hpp"

#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


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
