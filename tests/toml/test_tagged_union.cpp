#include "test_tagged_union.hpp"

#include <cassert>
#include <iostream>
#include <rfl.hpp>
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

using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const Shapes r = Rectangle{.height = 10, .width = 5};

  write_and_read(r);
}
}  // namespace test_tagged_union
