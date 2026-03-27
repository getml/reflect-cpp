#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <variant>

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

using Shapes = std::variant<Circle, Rectangle, Square>;

struct Drawing {
  Shapes shape;
};

TEST(boost_serialization, test_variant) {
  const auto drawing = Drawing{.shape = Circle{.radius = 5.0}};

  write_and_read(drawing);
}
}  // namespace test_variant
