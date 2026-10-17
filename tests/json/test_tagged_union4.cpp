#include <cassert>
#include <string>

#include "write_and_read.hpp"
#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


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
