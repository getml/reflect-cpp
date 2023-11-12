#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

void test_field_variant() {
  std::cout << "test_field_variant" << std::endl;

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

  const Shapes r =
      rfl::make_field<"rectangle">(Rectangle{.height = 10, .width = 5});

  write_and_read(r, R"({"rectangle":{"height":10.0,"width":5.0}})");
}
