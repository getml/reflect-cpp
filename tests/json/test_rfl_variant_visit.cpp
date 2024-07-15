#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_rfl_variant_visit {

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

using Shapes = rfl::Variant<Circle, Rectangle, std::unique_ptr<Square>>;

TEST(json, test_rfl_variant_visit) {
  Shapes r = Rectangle{.height = 10, .width = 5};
  Shapes c = Circle{.radius = 3};
  Shapes s = std::make_unique<Square>(3.0);
  const auto f = [](const auto& _r, const auto& _c, const auto& _s) {
    return rfl::json::write(_r) + ", " + rfl::json::write(_c) + ", " +
           rfl::json::write(_s);
  };
  const auto str = rfl::visit(f, r, c, s);
  std::cout << str << std::endl;
  EXPECT_TRUE(str ==
              R"({"height":10.0,"width":5.0}, {"radius":3.0}, {"width":3.0})");
}
}  // namespace test_rfl_variant_visit
