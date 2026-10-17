#include <cassert>
#include <string>

#include "write_and_read.hpp"
#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


namespace test_flag_enum1 {

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

TEST(json, test_flag_enum1) {
  const auto circle =
      Circle{.radius = 2.0, .color = Color::blue | Color::orange};

  write_and_read(circle, R"({"radius":2.0,"color":"red|blue|yellow"})");
}

}  // namespace test_flag_enum1
