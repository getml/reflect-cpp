#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

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

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

  const Shapes r = Rectangle{.height = 10, .width = 5};

  write_and_read(r);
}
}  // namespace test_tagged_union
