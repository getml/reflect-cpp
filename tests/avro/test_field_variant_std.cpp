#include <cassert>

#include "write_and_read.hpp"
#include <variant>

import rfl.avro;
import rfl;


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

TEST(avro, test_field_variant_std) {
  const Shapes r =
      rfl::make_field<"rectangle">(Rectangle{.height = 10, .width = 5});

  write_and_read_with_json(r);
}
}  // namespace test_field_variant_std
