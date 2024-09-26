#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_field_variant_std {

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

using Shapes = std::variant<rfl::Field<"circle", Circle>,
                            rfl::Field<"rectangle", Rectangle>,
                            rfl::Field<"square", rfl::Box<Square>>>;

TEST(json, test_field_variant_std) {
  const Shapes r =
      rfl::make_field<"rectangle">(Rectangle{.height = 10, .width = 5});

  write_and_read(r, R"({"rectangle":{"height":10.0,"width":5.0}})");
}
}  // namespace test_field_variant_std
