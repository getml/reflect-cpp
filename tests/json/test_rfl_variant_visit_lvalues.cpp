#include <cassert>

#include "write_and_read.hpp"
#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


namespace test_rfl_variant_visit_lvalues {

struct Rectangle {
  double height;
  double width;
};

struct Square {
  double width;
};

using Shapes = rfl::Variant<Rectangle, Square>;

TEST(json, test_rfl_variant) {
  const Shapes r = Rectangle{.height = 10, .width = 5};
  const auto get_width = [](const auto& _s) -> const double& {
    return _s.width;
  };
  EXPECT_EQ(rfl::visit(get_width, r), 5);
}
}  // namespace test_rfl_variant_visit_lvalues
