#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <type_traits>
#include <vector>

#include "rfl/internal/num_fields.hpp"
#include "test_replace.hpp"
#include "write_and_read.hpp"

namespace test_view {

struct Person {
  std::string first_name;
  std::string last_name;
  int age;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  auto lisa = Person{.first_name = "Lisa", .last_name = "Simpson", .age = 8};

  const auto view = rfl::to_view(lisa);

  *view.get<0>() = "Maggie";
  *std::get<1>(view.values()) = "Simpson";
  *view.get<"age">() = 0;

  write_and_read(lisa,
                 R"({"first_name":"Maggie","last_name":"Simpson","age":0})");
}
}  // namespace test_view
