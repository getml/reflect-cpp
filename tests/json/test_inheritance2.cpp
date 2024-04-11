#include <cassert>
#include <iostream>
#include <rfl.hpp>

#include "rfl/internal/num_fields.hpp"

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
struct EmptyDerived0 : BaseX, EmptyBase1 {};
struct EmptyDerived1 : EmptyBase1, BaseX {};
struct EmptyDerived2 : EmptyBase1, EmptyBase2, BaseX {};

void test() {
  Derived1 derived1;
  const auto derived1_view = rfl::to_view(derived1);
  static_assert(derived1_view.size() == 2);
  Derived2 derived2;
  const auto derived2_view = rfl::to_view(derived2);
  static_assert(derived2_view.size() == 3);

  EmptyDerived1 empty_derived0{rfl::internal::any(0), {}};
  auto empty_derived0_view = rfl::to_view(empty_derived0);
  static_assert(empty_derived0_view.size() == 2);

  EmptyDerived1 empty_derived1{rfl::internal::any(0), {}};
  auto empty_derived1_view = rfl::to_view(empty_derived1);
  static_assert(empty_derived1_view.size() == 2);

  EmptyDerived1 empty_derived2{rfl::internal::any(0), {}};
  auto empty_derived2_view = rfl::to_view(empty_derived2);
  static_assert(empty_derived0_view.size() == 2);

  std::cout << "OK\n";
}

}  // namespace test_inheritance