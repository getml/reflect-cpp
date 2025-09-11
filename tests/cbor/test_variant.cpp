#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

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

using Shape = std::variant<Circle, Rectangle, std::unique_ptr<Square>>;

TEST(cbor, test_variant) { 
  write_and_read(
    // Note: Non-integral values force double-precision encoding on cbor.me, for convenience
    Shape{Rectangle{.height = 10.00000001, .width = 5.00000001}},
    // Diagnostic notation: {_"height": 10.00000001, "width": 5.00000001}
    // From: https://cbor.me/?diag={_%22height%22:%2010.00000001,%20%22width%22:%205.00000001}
    R"(
BF                     # map(*)
   66                  # text(6)
      686569676874     # "height"
   FB 402400000055E63C # primitive(4621819117594601020)
   65                  # text(5)
      7769647468       # "width"
   FB 4014000000ABCC77 # primitive(4617315517972860023)
   FF                  # primitive(*)
    )");
}
}  // namespace test_variant
