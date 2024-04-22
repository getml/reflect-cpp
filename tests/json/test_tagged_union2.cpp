#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_tagged_union2 {

struct Circle {
  using Tag = rfl::Literal<"circle", "Circle">;
  rfl::Field<"radius", double> radius;
};

struct Rectangle {
  using Tag = rfl::Literal<"rectangle", "Rectangle">;
  rfl::Field<"height", double> height;
  rfl::Field<"width", double> width;
};

struct Square {
  using Tag = rfl::Literal<"square", "Square">;
  rfl::Field<"width", double> width;
};

using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

TEST(json, test_tagged_union2) {
  const Shapes r = Rectangle{.height = 10, .width = 5};

  write_and_read(r, R"({"shape":"rectangle","height":10.0,"width":5.0})");
}
}  // namespace test_tagged_union2
