#include <gtest/gtest.h>
// Redefine the range of the enums
#define RFL_ENUM_RANGE_MIN -128
#define RFL_ENUM_RANGE_MAX 128
#include <rfl.hpp>

namespace test_enum_range_size {

enum class InnerColor { none = -128, red = -50, green = 0, blue = 128 };
enum class LineColor { yellow = 200, purple = 300, orange = 400 };
enum class OneColor {black};

}  // namespace test_enum_range_size

// Define the range of the LineColor enum
template <>
struct rfl::config::enum_range<test_enum_range_size::LineColor> {
  static constexpr int min = 0;
  static constexpr int max = 400;
};

// Define the range of the OneColor enum
template <>
struct rfl::config::enum_range<test_enum_range_size::OneColor> {
  static constexpr int min = 0;
  static constexpr int max = 0;
};

namespace test_enum_range_size {

TEST(generic, test_enum_range_size) {
  EXPECT_EQ(rfl::get_enumerator_array<InnerColor>().size(), 4);
  EXPECT_EQ(rfl::get_enumerator_array<LineColor>().size(), 3);
  EXPECT_EQ(rfl::get_enumerator_array<OneColor>().size(), 1);
}

}  // namespace test_enum_range_size