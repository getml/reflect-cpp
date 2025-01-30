#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

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

struct Shapes {
  rfl::Variant<rfl::Field<"circle", Circle>, rfl::Field<"rectangle", Rectangle>,
               rfl::Field<"square", rfl::Box<Square>>>
      root;
};

TEST(capnproto, test_field_variant_std) {
  const auto r =
      Shapes{rfl::make_field<"rectangle">(Rectangle{.height = 10, .width = 5})};

  write_and_read(r);
}
}  // namespace test_field_variant
