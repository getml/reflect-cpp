#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_as {

struct A {
  rfl::Field<"f1", std::string> f1;
  rfl::Field<"f2", rfl::Box<std::string>> f2;
};

struct B {
  rfl::Field<"f3", std::string> f3;
  rfl::Field<"f4", rfl::Box<std::string>> f4;
};

struct C {
  rfl::Field<"f1", std::string> f1;
  rfl::Field<"f2", rfl::Box<std::string>> f2;
  rfl::Field<"f4", rfl::Box<std::string>> f4;
};

TEST(json, test_as) {

  auto a = A{.f1 = "Hello", .f2 = rfl::make_box<std::string>("World")};

  auto b = B{.f3 = "Hello", .f4 = rfl::make_box<std::string>("World")};

  const auto c = rfl::as<C>(std::move(a), std::move(b));

  write_and_read(c, R"({"f1":"Hello","f2":"World","f4":"World"})");
}

}  // namespace test_as
