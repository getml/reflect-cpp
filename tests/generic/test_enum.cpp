#include <cassert>

#include "write_and_read.hpp"
#include <test_rfl/rfl.hpp>


namespace test_enum {

enum class Color { red, green, blue, yellow };
enum Location { left, centre, right };  // Unscoped enum

struct Circle {
  float radius;
  Color color;
  Location location;
};

TEST(generic, test_enum) {
  const auto circle =
      Circle{.radius = 2.0, .color = Color::green, .location = centre};

  write_and_read(circle);
}

}  // namespace test_enum
