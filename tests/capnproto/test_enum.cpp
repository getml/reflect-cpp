#include <cassert>

#include "write_and_read.hpp"
#include <test_rfl/rfl.capnproto.hpp>
#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


namespace test_enum {

enum class Color { red, green, blue, yellow };

struct Circle {
  float radius;
  Color color;
};

TEST(capnproto, test_enum) {
  const auto circle = Circle{.radius = 2.0, .color = Color::green};

  write_and_read(circle);
}

}  // namespace test_enum
