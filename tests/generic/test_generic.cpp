#include <gtest/gtest.h>

#include <climits>
#include <cstdint>

#include <rfl.hpp>

namespace test_generic_to_int_truncation {

TEST(regression, generic_to_int_rejects_overflow) {
  const int64_t value = static_cast<int64_t>(INT_MAX) + 1;
  const auto big = rfl::Generic(value);
  const auto result = big.to_int();
  EXPECT_FALSE(result)
      << "Generic::to_int() should fail for value " << value
      << " exceeding INT_MAX";
}

TEST(regression, generic_to_int_rejects_underflow) {
  const int64_t value = static_cast<int64_t>(INT_MIN) - 1;
  const auto small = rfl::Generic(value);
  const auto result = small.to_int();
  EXPECT_FALSE(result)
      << "Generic::to_int() should fail for value " << value
      << " below INT_MIN";
}

}  // namespace test_generic_to_int_truncation
