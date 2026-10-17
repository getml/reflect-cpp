#include <cassert>

#include "write_and_read.hpp"
#include <test_rfl/rfl.toml.hpp>
#include <test_rfl/rfl.hpp>


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

TEST(toml, test_field_variant) {
  const Shapes r =
      rfl::make_field<"rectangle">(Rectangle{.height = 10, .width = 5});

  write_and_read(r);
}
}  // namespace test_field_variant
