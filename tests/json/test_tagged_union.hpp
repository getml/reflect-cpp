#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_tagged_union {

struct Circle {
  using Tag = rfl::Literal<"Circle">;
  rfl::Field<"radius", double> radius;
};

struct Rectangle {
  using Tag = rfl::Literal<"Rectangle">;
  rfl::Field<"height", double> height;
  rfl::Field<"width", double> width;
};

struct Square {
  using Tag = rfl::Literal<"Square">;
  rfl::Field<"width", double> width;
};

using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

void test() {
  std::cout << "test_tagged_union" << std::endl;

  const Shapes r = Rectangle{.height = 10, .width = 5};

  write_and_read(r, R"({"shape":"Rectangle","height":10.0,"width":5.0})");
}
}  // namespace test_tagged_union
