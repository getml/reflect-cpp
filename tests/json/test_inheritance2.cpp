#include <cassert>
#include <iostream>
#include <rfl.hpp>

namespace test_inheritance2 {

struct EmptyBase1 {};
struct EmptyBase2 {};
struct Derived1 : public EmptyBase1 {
  int x;
  int y;
};
struct Derived2 : public EmptyBase1, public EmptyBase2 {
  int x;
  int y;
  int z;
};

struct BaseX {
  int x;
  int y;
};
struct EmptyDerived : public BaseX {};

void test() {
  Derived1 derived1;
  const auto derived1_view = rfl::to_view(derived1);
  static_assert(derived1_view.size() == 2);
  Derived2 derived2;
  const auto derived2_view = rfl::to_view(derived2);
  static_assert(derived2_view.size() == 3);

  EmptyDerived empty_derived;
  auto empty_derived_view = rfl::to_view(empty_derived);
  static_assert(empty_derived_view.size() == 2);

  std::cout << "OK\n";
}

}  // namespace test_inheritance