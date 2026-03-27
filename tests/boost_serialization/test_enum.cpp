#include <rfl.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_enum {

enum class Color { red, green, blue };

struct Circle {
  float radius;
  Color color;
};

TEST(boost_serialization, test_enum) {
  const auto circle = Circle{.radius = 2.0f, .color = Color::green};

  write_and_read_with_json(circle);
}
}  // namespace test_enum
