#include <gtest/gtest.h>

#include <cassert>
#include <rfl.hpp>
#include <rfl/env.hpp>

namespace test_enum {

enum class Color { red, green, blue, yellow };

struct Circle {
  float radius;
  Color color;
};

TEST(env, test_enum) {
  const auto circle = Circle{.radius = 2.0, .color = Color::green};
}

}  // namespace test_enum
