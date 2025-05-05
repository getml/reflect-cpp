#include <gtest/gtest.h>
// Redefine the range of the enums
#define RFL_ENUM_RANGE_MIN -128
#define RFL_ENUM_RANGE_MAX 128
#include <rfl.hpp>

namespace test_enum_range_min_max {

enum class InnerColor { none = -128, red = -50, green = 0, blue = 128 };
enum class LineColor : uint16_t { yellow = 200, purple = 300, orange = 400 };

}  // namespace test_enum_range_min_max

// Define the range of the LineColor enum
template <>
struct rfl::config::enum_range<test_enum_range_min_max::LineColor> {
  static constexpr int min = 200;
  static constexpr int max = 400;
};

namespace test_enum_range_min_max {

TEST(generic, test_enum_range_min_max) {
  auto [inner_min, inner_max] = rfl::get_enum_range<InnerColor>();
  EXPECT_EQ(inner_min, RFL_ENUM_RANGE_MIN);
  EXPECT_EQ(inner_max, RFL_ENUM_RANGE_MAX);

  auto [line_min, line_max] = rfl::get_enum_range<LineColor>();
  EXPECT_EQ(line_min, 200);
  EXPECT_EQ(line_max, 400);
}

}  // namespace test_enum_range_min_max
