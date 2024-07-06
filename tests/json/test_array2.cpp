
#include <array>
#include <rfl.hpp>
#include <variant>

#include "write_and_read.hpp"

namespace test_array2 {

struct Foo {
  std::array<int, 2> v;
};

struct Bar {
  std::array<int, 3> v;
};

using FooBar = std::variant<Foo, Bar>;  // or rfl::Variant

TEST(json, test_array2) {
  FooBar foobar = Bar({1, 2, 3});

  write_and_read(foobar, R"({"v":[1,2,3]})");
}
}  // namespace test_array2
