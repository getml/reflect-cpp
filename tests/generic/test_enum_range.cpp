#include <cassert>
// Redefine the range of the enums
#define RFL_ENUM_RANGE_MIN -128
#define RFL_ENUM_RANGE_MAX 128
#include <rfl.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_enum_range {

enum class InnerColor { none = -128, red = -50, green = 0, blue = 128 };
enum class LineColor { yellow = 200, purple = 300, orange = 400 };

struct Circle {
  float radius;
  InnerColor inner_color;
  LineColor line_color;
};

}  // namespace test_enum_range

// Define the range of the LineColor enum
template <>
struct rfl::config::enum_range<test_enum_range::LineColor> {
  static constexpr int min = 200;
  static constexpr int max = 400;
};

namespace test_enum_range {

TEST(generic, test_enum_range) {
  auto circle = Circle{.radius = 2.0,
                       .inner_color = InnerColor::none,
                       .line_color = LineColor::yellow};

  write_and_read(circle);
}

}  // namespace test_enum_range