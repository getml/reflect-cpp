#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_as_flatten {

struct A {
  std::string f1;
  rfl::Box<std::string> f2;
};

struct B {
  std::string f3;
  rfl::Box<std::string> f4;
};

struct C {
  rfl::Flatten<A> a;
  rfl::Flatten<B> b;
  int f5;
};

TEST(json, test_as_flatten) {
  auto a = A{.f1 = "Hello", .f2 = rfl::make_box<std::string>("World")};

  auto b = B{.f3 = "Hello", .f4 = rfl::make_box<std::string>("World")};

  const auto c =
      rfl::as<C>(std::move(a), std::move(b), rfl::make_field<"f5">(5));

  write_and_read(
      c, R"({"f1":"Hello","f2":"World","f3":"Hello","f4":"World","f5":5})");
}
}  // namespace test_as_flatten
