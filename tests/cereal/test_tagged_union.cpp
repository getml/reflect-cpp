#include <iostream>
#include <string>
#include <vector>

#include "write_and_read.hpp"
import rfl;
import rfl.cereal;


namespace test_tagged_union {

struct Circle {
  using Tag = rfl::Literal<"circle">;
  double radius;
};

struct Rectangle {
  using Tag = rfl::Literal<"rectangle">;
  double height;
  double width;
};

struct Square {
  using Tag = rfl::Literal<"square">;
  double width;
};

using Shapes = rfl::TaggedUnion<"shape", Circle, Rectangle, Square>;

TEST(cereal, test_tagged_union) {
  const Shapes r = Rectangle{.height = 10, .width = 5};
  write_and_read(r);
}

}  // namespace test_tagged_union
