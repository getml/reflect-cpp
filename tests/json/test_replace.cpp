#include "test_replace.hpp"

#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_replace {

struct Person {
  rfl::Field<"firstName", std::string> first_name;
  rfl::Field<"lastName", std::string> last_name;
  rfl::Field<"children", rfl::Box<std::vector<Person>>> children;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  auto lisa = Person{.first_name = "Lisa",
                     .last_name = "Simpson",
                     .children = rfl::make_box<std::vector<Person>>()};

  const auto maggie = rfl::replace(
      std::move(lisa), rfl::make_field<"firstName">(std::string("Maggie")));

  write_and_read(
      maggie, R"({"firstName":"Maggie","lastName":"Simpson","children":[]})");
}
}  // namespace test_replace
