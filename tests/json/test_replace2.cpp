#include <cassert>
#include <string>
#include <vector>

#include "write_and_read.hpp"
#include <utility>

import rfl;
import rfl.json;


namespace test_replace2 {

struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Box<std::vector<Person>> children;
};

TEST(json, test_replace2) {
  auto lisa = Person{.first_name = "Lisa",
                     .last_name = "Simpson",
                     .children = rfl::make_box<std::vector<Person>>()};

  const auto maggie = rfl::replace(
      std::move(lisa), rfl::make_field<"first_name">(std::string("Maggie")));

  write_and_read(
      maggie, R"({"first_name":"Maggie","last_name":"Simpson","children":[]})");
}
}  // namespace test_replace2
