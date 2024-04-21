#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <source_location>

namespace test_inheritance {

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  struct S {
    int x;
  };

  struct T : S {};

  constexpr auto name = get<0>(rfl::fields<T>()).name();

  static_assert(name == "x");

  std::cout << "OK" << std::endl << std::endl;
}

}  // namespace test_inheritance
