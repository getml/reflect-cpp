#include <cassert>
#include <string>

#include <memory>
#include <variant>

#include "write_and_read.hpp"

#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


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

TEST(json, test_variant) {
  const Shapes r = Rectangle{.height = 10, .width = 5};

  write_and_read(r, R"({"height":10.0,"width":5.0})");
}
}  // namespace test_variant
