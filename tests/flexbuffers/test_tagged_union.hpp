#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

// Makes sure that we can set default values.
void test_tagged_union() {
  std::cout << "test_tagged_union" << std::endl;

  struct Circle {
    rfl::Field<"shape", rfl::Literal<"Circle">> shape = rfl::default_value;
    rfl::Field<"radius", double> radius;
  };

  struct Rectangle {
    rfl::Field<"shape", rfl::Literal<"Rectangle">> shape = rfl::default_value;
    rfl::Field<"height", double> height;
    rfl::Field<"width", double> width;
  };

  struct Square {
    rfl::Field<"shape", rfl::Literal<"Square">> shape = rfl::default_value;
    rfl::Field<"width", double> width;
  };

  using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

  const Shapes r = Rectangle{.height = 10, .width = 5};

  write_and_read(r);
}
