#include <rfl.hpp>
#include <rfl/json.hpp>

#include "write_and_read.hpp"

namespace test_tagged_union_with_no_extra_fields {

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

using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

TEST(json, test_tagged_union_with_no_extra_fields) {
  const Shapes r = Rectangle{.height = 10, .width = 5};

  write_and_read<rfl::NoExtraFields>(
      r, R"({"shape":"Rectangle","height":10.0,"width":5.0})");
}
}  // namespace test_tagged_union_with_no_extra_fields
