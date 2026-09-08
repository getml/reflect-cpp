// Flatten::operator=(Flatten<U>&&) uses std::forward<U>(_f) which passes
// Flatten<U> instead of U — fails to compile for cross-type assignment.
// File: include/rfl/Flatten.hpp:103-105
// Should be `value_ = std::move(_f.get())`.
//
// This file instantiates the buggy operator. It should FAIL to compile
// while the bug exists, and succeed after the fix.

#include <rfl.hpp>

struct Base {
  int x = 0;
};

struct Derived {
  int x = 0;
  operator Base() const { return Base{x}; }
};

int main() {
  auto a = rfl::Flatten<Base>(Base{});
  auto b = rfl::Flatten<Derived>(Derived{});
  a = std::move(b);
  return a.get().x;
}
