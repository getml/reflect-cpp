#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <type_traits>
#include <vector>

#include "write_and_read.hpp"

namespace test_and_then {

struct Person {
  std::string first_name;
  std::string last_name;
  int age;
};

TEST(json, test_and_then) {
  const auto lisa =
      Person{.first_name = "Lisa", .last_name = "Simpson", .age = 8};

  const auto to_bart = [](auto field) {
    if constexpr (decltype(field)::name() == "first_name") {
      field = "Bart";
      return rfl::make_named_tuple(field);
    } else if constexpr (decltype(field)::name() == "age") {
      return rfl::make_named_tuple();
    } else {
      return rfl::make_named_tuple(field);
    }
  };

  const auto bart = rfl::to_named_tuple(lisa).and_then(to_bart);

  write_and_read(bart, R"({"first_name":"Bart","last_name":"Simpson"})");
}

}  // namespace test_and_then
