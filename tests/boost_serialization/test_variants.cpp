#include <rfl.hpp>
#include <string>
#include <variant>

#include "write_and_read.hpp"

namespace test_variants {

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

TEST(boost_serialization, test_std_variant) {
  const auto drawing = Drawing{.shape = Circle{.radius = 5.0}};
  write_and_read(drawing);
}

TEST(boost_serialization, test_std_variant_rectangle) {
  const auto drawing = Drawing{.shape = Rectangle{.height = 10, .width = 5}};
  write_and_read(drawing);
}

}  // namespace test_variants
