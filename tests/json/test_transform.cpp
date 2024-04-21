#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <type_traits>
#include <vector>

#include "write_and_read.hpp"

namespace test_transform {

struct Person {
  std::string first_name;
  std::string last_name;
  int age;
};

TEST(json, test_transform) {
  const auto lisa =
      Person{.first_name = "Lisa", .last_name = "Simpson", .age = 8};

  const auto to_bart = [](auto field) {
    if constexpr (decltype(field)::name() == "first_name") {
      field = "Bart";
      return field;
    } else if constexpr (decltype(field)::name() == "age") {
      field = 10;
      return field;
    } else {
      return field;
    }
  };

  const auto bart = rfl::to_named_tuple(lisa).transform(to_bart);

  write_and_read(bart,
                 R"({"first_name":"Bart","last_name":"Simpson","age":10})");
}

}  // namespace test_transform
