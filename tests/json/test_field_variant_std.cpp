#include <cassert>
#include <string>

#include "write_and_read.hpp"
#include <variant>

import rfl;
import rfl.json;


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
