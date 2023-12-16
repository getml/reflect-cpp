#include "test_field_variant.hpp"

#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_field_variant {

struct Circle {
  rfl::Field<"radius", double> radius;
};

struct Rectangle {
  rfl::Field<"height", double> height;
  rfl::Field<"width", double> width;
};

struct Square {
  rfl::Field<"width", double> width;
};

using Shapes = rfl::Variant<rfl::Field<"circle", Circle>,
                            rfl::Field<"rectangle", Rectangle>,
                            rfl::Field<"square", rfl::Box<Square>>>;

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const Shapes r =
      rfl::make_field<"rectangle">(Rectangle{.height = 10, .width = 5});

  write_and_read(r);
}
}  // namespace test_field_variant
