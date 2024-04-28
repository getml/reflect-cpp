#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace test_enum7 {

enum class Color { red, green, blue, yellow };

struct Circle {
  float radius;
  Color color;
};

TEST(json, test_enum7) {
  constexpr auto enumerator_array = rfl::get_enumerator_array<Color>();
  static_assert(enumerator_array[0].first == "red");
  static_assert(enumerator_array[1].first == "green");
  static_assert(enumerator_array[2].first == "blue");
  static_assert(enumerator_array[3].first == "yellow");
  static_assert(enumerator_array[0].second == Color::red);
  static_assert(enumerator_array[1].second == Color::green);
  static_assert(enumerator_array[2].second == Color::blue);
  static_assert(enumerator_array[3].second == Color::yellow);

  constexpr auto enumerator_array_underlying =
      rfl::get_underlying_enumerator_array<Color>();
  static_assert(enumerator_array_underlying[0].first == "red");
  static_assert(enumerator_array_underlying[1].first == "green");
  static_assert(enumerator_array_underlying[2].first == "blue");
  static_assert(enumerator_array_underlying[3].first == "yellow");
  static_assert(enumerator_array_underlying[0].second == 0);
  static_assert(enumerator_array_underlying[1].second == 1);
  static_assert(enumerator_array_underlying[2].second == 2);
  static_assert(enumerator_array_underlying[3].second == 3);

  // This is a compile-time test
  EXPECT_TRUE(true);
}

}  // namespace test_enum7
