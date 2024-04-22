#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <tuple>

#include <gtest/gtest.h>

namespace test_inheritance {

TEST(json, test_inheritance) {
  struct S {
    int x;
  };

  struct T : S {};

  constexpr auto name =
      std::tuple_element_t<0, typename rfl::named_tuple_t<T>::Fields>::name();

  static_assert(name == "x");

  EXPECT_TRUE(true);
}

}  // namespace test_inheritance
