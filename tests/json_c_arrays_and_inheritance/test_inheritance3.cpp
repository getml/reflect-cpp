#ifdef REFLECTCPP_USE_CPP_REFLECTION

// Only supported with C++26 reflection

#include <gtest/gtest.h>

#include <cassert>
#include <rfl.hpp>

#include "rfl/num_fields.hpp"

namespace test_inheritance3 {

struct Base {
  int x;
};

struct Derived : public Base {
  int y;
};

TEST(json, test_inheritance3) {
  Derived derived;
  const auto derived_view = rfl::to_view(derived);

  constexpr auto name_x =
      rfl::tuple_element_t<0, typename rfl::named_tuple_t<T>::Fields>::name();
  constexpr auto name_y =
      rfl::tuple_element_t<1, typename rfl::named_tuple_t<T>::Fields>::name();

  static_assert(name_x == "x");
  static_assert(name_y == "y");

  EXPECT_TRUE(true);
}

}  // namespace test_inheritance3

#endif
