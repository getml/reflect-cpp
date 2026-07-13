#include <gtest/gtest.h>

#include <cassert>
#include <rfl.hpp>
#include <rfl/env.hpp>

namespace test_tagged_union2 {

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

using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

TEST(env, test_tagged_union2) {
  const Shapes r = Rectangle{.height = 10, .width = 5};
}
}  // namespace test_tagged_union2
