#include <cassert>
#include <iostream>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_rfl_variant_visit_move_only {

struct Rectangle {
  double height;
  std::unique_ptr<double> width;
};

struct Square {
  std::unique_ptr<double> width;
};

struct Circle {
  double const radius;
};

using Shapes = rfl::Variant<Rectangle, Square>;

TEST(json, test_rfl_variant_visit_move_only) {
  const Shapes r =
      Rectangle{.height = 10, .width = std::make_unique<double>(5.0)};
  const auto get_width = [](const auto& _s) -> const std::unique_ptr<double>& {
    return _s.width;
  };
  EXPECT_EQ(*rfl::visit(get_width, r), 5.0);
}

TEST(json, test_rfl_variant_visit_return_move_only) {
  auto variant = rfl::Variant<Circle>{Circle{.radius = 10}};
  const auto get_radius = [](const auto& object) -> const double& {
    return object.radius;
  };
  EXPECT_EQ(rfl::visit(get_radius, variant), 10.0);
}
}  // namespace test_rfl_variant_visit_move_only
