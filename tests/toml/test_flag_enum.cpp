#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_flag_enum {

enum class Color {
  red = 256,
  green = 512,
  blue = 1024,
  yellow = 2048,
  orange = (256 | 2048)  // red + yellow = orange
};

inline Color operator|(Color c1, Color c2) {
  return static_cast<Color>(static_cast<int>(c1) | static_cast<int>(c2));
}

struct Circle {
  float radius;
  Color color;
};

TEST(toml, test_flag_enum) {
  const auto circle =
      Circle{.radius = 2.0, .color = Color::blue | Color::orange};

  write_and_read(circle);
}

}  // namespace test_flag_enum
