#include <gtest/gtest.h>

#include <cassert>
#include <rfl.hpp>
#include <rfl/env.hpp>

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

TEST(env, test_variant) {
  const Shapes r = Rectangle{.height = 10, .width = 5};
}
}  // namespace test_variant
