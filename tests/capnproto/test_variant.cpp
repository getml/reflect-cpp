#include <cassert>

#include <memory>
#include <variant>

#include "write_and_read.hpp"

#include <test_rfl/rfl.capnproto.hpp>
#include <test_rfl/rfl.hpp>


namespace test_variant {

struct Circle {
  double radius;
};

struct Rectangle {
  double height;
  double width;
};

struct Square {
  double width;
};

struct Shapes {
  std::variant<Circle, Rectangle, std::unique_ptr<Square>> root;
};

TEST(capnproto, test_variant) {
  const auto r = Shapes{Rectangle{.height = 10, .width = 5}};

  write_and_read(r);
}
}  // namespace test_variant
