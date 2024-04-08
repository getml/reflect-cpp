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

  const auto name = get<0>(rfl::fields<T>()).name();
  if (name == "x") {
    std::cout << "OK" << std::endl;
  } else {
    std::cout << "FAIL\n"
              << "Expected member name 'x', got '" << name << "'" << std::endl;
  }
}

}  // namespace test_inheritance