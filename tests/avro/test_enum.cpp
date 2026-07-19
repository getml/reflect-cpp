#include <cassert>

#include "write_and_read.hpp"
import rfl.avro;
import rfl;
import rfl.json;


namespace test_enum {

enum class Color { red, green, blue, yellow };

struct Circle {
  float radius;
  Color color;
};

TEST(avro, test_enum) {
  const auto circle = Circle{.radius = 2.0, .color = Color::green};

  write_and_read_with_json(circle);
}

}  // namespace test_enum
