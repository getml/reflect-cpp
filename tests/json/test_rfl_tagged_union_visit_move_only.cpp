#include <cassert>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>

#include "rfl/TaggedUnion.hpp"
#include "write_and_read.hpp"

namespace test_rfl_tagged_union_visit_move_only {

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

using Shapes = rfl::TaggedUnion<"Shapes", Rectangle, Square>;

TEST(json, test_rfl_tagged_union_visit_move_only) {
  const Shapes r =
      Rectangle{.height = 10, .width = std::make_unique<double>(5.0)};
  const auto get_width = [](const auto& _s) -> const std::unique_ptr<double>& {
    return _s.width;
  };
  EXPECT_EQ(*rfl::visit(get_width, r), 5.0);
}

TEST(json, test_rfl_tagged_union_visit_return_move_only_const) {
  auto const circle = Circle{.radius = 10};
  auto const tagged_union = rfl::TaggedUnion<"Circle", Circle>{circle};
  const auto get_radius = [](const auto& object) -> const double& {
    return object.radius;
  };
  auto const result = rfl::visit(get_radius, tagged_union);
  EXPECT_EQ(result, circle.radius);
}

TEST(json, test_rfl_tagged_union_visit_return_move_only) {
  auto circle = Circle{.radius = 10};
  auto tagged_union = rfl::TaggedUnion<"Circle", Circle>{circle};
  auto const get_radius = [](auto& object) -> double const& {
    return object.radius;
  };
  auto result = rfl::visit(get_radius, tagged_union);
  EXPECT_EQ(result, circle.radius);
}
}  // namespace test_rfl_tagged_union_visit_move_only
