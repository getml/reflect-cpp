#include <gtest/gtest.h>

#include <compare>
#include <limits>
#include <string>

#include <rfl.hpp>

namespace test_tuple_spaceship {

TEST(Tuple, spaceship_single_less) {
  const auto a = rfl::Tuple<int>(1);
  const auto b = rfl::Tuple<int>(2);
  EXPECT_EQ(a <=> b, std::strong_ordering::less);
}

TEST(Tuple, spaceship_single_greater) {
  const auto a = rfl::Tuple<int>(3);
  const auto b = rfl::Tuple<int>(1);
  EXPECT_EQ(a <=> b, std::strong_ordering::greater);
}

TEST(Tuple, spaceship_single_equivalent) {
  const auto a = rfl::Tuple<int>(5);
  const auto b = rfl::Tuple<int>(5);
  EXPECT_EQ(a <=> b, std::strong_ordering::equivalent);
}

TEST(Tuple, spaceship_multi_first_differs) {
  const auto a = rfl::Tuple<int, int, int>(1, 10, 10);
  const auto b = rfl::Tuple<int, int, int>(2, 10, 10);
  EXPECT_EQ(a <=> b, std::strong_ordering::less);
}

TEST(Tuple, spaceship_multi_second_differs) {
  const auto a = rfl::Tuple<int, int, int>(1, 2, 10);
  const auto b = rfl::Tuple<int, int, int>(1, 3, 10);
  EXPECT_EQ(a <=> b, std::strong_ordering::less);
}

TEST(Tuple, spaceship_multi_last_differs) {
  const auto a = rfl::Tuple<int, int, int>(1, 2, 3);
  const auto b = rfl::Tuple<int, int, int>(1, 2, 4);
  EXPECT_EQ(a <=> b, std::strong_ordering::less);
}

TEST(Tuple, spaceship_multi_all_equivalent) {
  const auto a = rfl::Tuple<int, int, int>(1, 2, 3);
  const auto b = rfl::Tuple<int, int, int>(1, 2, 3);
  EXPECT_EQ(a <=> b, std::strong_ordering::equivalent);
}

TEST(Tuple, spaceship_multi_greater_first_decides) {
  const auto a = rfl::Tuple<int, int>(5, 1);
  const auto b = rfl::Tuple<int, int>(3, 9);
  EXPECT_EQ(a <=> b, std::strong_ordering::greater);
}

TEST(Tuple, spaceship_multi_greater_later_element) {
  const auto a = rfl::Tuple<int, int, int>(1, 2, 5);
  const auto b = rfl::Tuple<int, int, int>(1, 2, 3);
  EXPECT_EQ(a <=> b, std::strong_ordering::greater);
}

TEST(Tuple, spaceship_partial_ordering) {
  const auto a = rfl::Tuple<double>(1.0);
  const auto b = rfl::Tuple<double>(2.0);
  EXPECT_EQ(a <=> b, std::partial_ordering::less);
}

TEST(Tuple, spaceship_nan_unordered) {
  const auto a = rfl::Tuple<double>(std::numeric_limits<double>::quiet_NaN());
  const auto b = rfl::Tuple<double>(1.0);
  EXPECT_EQ(a <=> b, std::partial_ordering::unordered);
}

TEST(Tuple, spaceship_lexicographic_order) {
  // First element decides, even if later elements disagree.
  const auto a = rfl::Tuple<int, int>(1, 100);
  const auto b = rfl::Tuple<int, int>(2, 0);
  EXPECT_EQ(a <=> b, std::strong_ordering::less);
}

TEST(Tuple, spaceship_mixed_types) {
  const auto a = rfl::Tuple<int, std::string>(1, "abc");
  const auto b = rfl::Tuple<int, std::string>(1, "abd");
  EXPECT_EQ(a <=> b, std::strong_ordering::less);
}

TEST(Tuple, spaceship_mixed_types_first_decides) {
  const auto a = rfl::Tuple<int, std::string>(2, "aaa");
  const auto b = rfl::Tuple<int, std::string>(1, "zzz");
  EXPECT_EQ(a <=> b, std::strong_ordering::greater);
}

TEST(Tuple, equality_single) {
  const auto a = rfl::Tuple<int>(42);
  const auto b = rfl::Tuple<int>(42);
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a == rfl::Tuple<int>(43));
}

TEST(Tuple, equality_multi) {
  const auto a = rfl::Tuple<int, std::string>(1, "hello");
  const auto b = rfl::Tuple<int, std::string>(1, "hello");
  const auto c = rfl::Tuple<int, std::string>(1, "world");
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a == c);
}

}  // namespace test_tuple_spaceship