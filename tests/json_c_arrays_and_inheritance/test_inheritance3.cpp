#ifdef REFLECTCPP_USE_CPP26_REFLECTION

// Only supported with C++26 reflection

#include <gtest/gtest.h>

#include <cassert>
#include <rfl.hpp>

#include "write_and_read.hpp"

namespace test_inheritance3 {

struct Base {
  int x;
};

struct Derived : public Base {
  int y;
};

TEST(json, test_inheritance3) {
  // Inheritance does not mix with designated initializers.
  const auto derived = Derived{1, 2};
  write_and_read(derived, R"({"x":1,"y":2})");
}

}  // namespace test_inheritance3

#endif
