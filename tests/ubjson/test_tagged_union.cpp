#include <cassert>

#include "write_and_read.hpp"
import rfl.ubjson;
import rfl;


namespace test_tagged_union {

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

using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

TEST(ubjson, test_tagged_union) { 
  const Shapes r = Rectangle{.height = 10, .width = 5};
  write_and_read(r);
}
}  // namespace test_tagged_union
