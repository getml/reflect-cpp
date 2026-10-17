#include <cassert>

#include "write_and_read.hpp"
#include <test_rfl/rfl.msgpack.hpp>
#include <test_rfl/rfl.hpp>


namespace test_tagged_union2 {

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

TEST(msgpack, test_tagged_union2) {
  const Shapes r = Rectangle{.height = 10, .width = 5};
  write_and_read<rfl::NoFieldNames>(r);
}
}  // namespace test_tagged_union2
