#include "test_variant.hpp"

#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_variant {

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

using Shapes = std::variant<Circle, Rectangle, std::unique_ptr<Square>>;

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const Shapes r = Rectangle{.height = 10, .width = 5};

  write_and_read(r);
}
}  // namespace test_variant
