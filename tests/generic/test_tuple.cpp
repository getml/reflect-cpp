#include <gtest/gtest.h>

#include <compare>

#include <rfl.hpp>

namespace test_tuple_spaceship {

TEST(regression, tuple_spaceship_not_always_equivalent) {
  const auto a = rfl::Tuple<int>(1);
  const auto b = rfl::Tuple<int>(2);
  EXPECT_NE(a <=> b, std::strong_ordering::equivalent)
      << "Tuple<int>(1) <=> Tuple<int>(2) should not be equivalent";
}

TEST(regression, tuple_spaceship_less) {
  const auto a = rfl::Tuple<int>(1);
  const auto b = rfl::Tuple<int>(2);
  EXPECT_EQ(a <=> b, std::strong_ordering::less)
      << "Tuple<int>(1) <=> Tuple<int>(2) should be less";
}

}  // namespace test_tuple_spaceship
