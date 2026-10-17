#include <cassert>

#include "write_and_read.hpp"
#include <test_rfl/rfl.avro.hpp>
#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


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
