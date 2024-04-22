#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_tagged_union3 {

struct Circle {
  rfl::Literal<"circle", "Circle"> shape;
  double radius;
};

struct Rectangle {
  rfl::Literal<"rectangle", "Rectangle", "rect"> shape;
  double height;
  double width;
};

struct Square {
  rfl::Literal<"square", "Square"> shape;
  double width;
};

struct Empty {};

using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle, Empty>;

TEST(json, test_tagged_union3) {
  const Shapes r = Rectangle{
      .shape = rfl::Literal<"rectangle", "Rectangle", "rect">::make<"rect">(),
      .height = 10,
      .width = 5};

  write_and_read(r, R"({"shape":"rect","height":10.0,"width":5.0})");
}
}  // namespace test_tagged_union3
