#include <gtest/gtest.h>

#include <cassert>
#include <rfl.hpp>
#include <rfl/env.hpp>

namespace test_field_variant {

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

using Shapes = rfl::Variant<rfl::Field<"circle", Circle>,
                            rfl::Field<"rectangle", Rectangle>,
                            rfl::Field<"square", rfl::Box<Square>>>;

TEST(env, test_field_variant) {
  const Shapes r =
      rfl::make_field<"rectangle">(Rectangle{.height = 10, .width = 5});
}
}  // namespace test_field_variant
