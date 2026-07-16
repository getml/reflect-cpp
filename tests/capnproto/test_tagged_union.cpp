#include <cassert>

#include "write_and_read.hpp"
import rfl.capnproto;
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

struct Shapes {
  rfl::TaggedUnion<"shape", Rectangle, Circle, Square> root;
};

TEST(capnproto, test_tagged_union) {
  const auto r = Shapes{Rectangle{.height = 10, .width = 5}};
  write_and_read(r);
}
}  // namespace test_tagged_union
