#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_variant {

struct Circle {
  rfl::Field<"radius", double> radius;
};

struct Rectangle {
  rfl::Field<"height", double> height;
  rfl::Field<"width", double> width;
};

struct Square {
  rfl::Field<"width", double> width;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  using Shapes = std::variant<Circle, Rectangle, std::unique_ptr<Square>>;

  const Shapes r = Rectangle{.height = 10, .width = 5};

  write_and_read(r);
}
}  // namespace test_variant
