#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_tagged_union4 {

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

TEST(json, test_tagged_union4) {
  const Shapes r = Rectangle{.height = 10, .width = 5};

  write_and_read<rfl::NoFieldNames>(r, R"(["Rectangle",10.0,5.0])");
}
}  // namespace test_tagged_union4
