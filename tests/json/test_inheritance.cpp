#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <tuple>

namespace test_inheritance {

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  struct S {
    int x;
  };

  struct T : S {};

  constexpr auto name =
      std::tuple_element_t<0, typename rfl::named_tuple_t<T>::Fields>::name();

  static_assert(name == "x");

  std::cout << "OK" << std::endl << std::endl;
}

}  // namespace test_inheritance
