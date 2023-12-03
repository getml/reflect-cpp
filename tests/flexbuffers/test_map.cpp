#include "test_map.hpp"

#include <iostream>
#include <map>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <source_location>
#include <string>

#include "write_and_read.hpp"

namespace test_map {

struct Person {
  rfl::Field<"firstName", std::string> first_name;
  rfl::Field<"lastName", std::string> last_name = "Simpson";
  rfl::Field<"children", std::unique_ptr<std::map<int, Person>>> children =
      rfl::default_value;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  auto children = std::make_unique<std::map<int, Person>>();
  children->insert(std::make_pair(1, Person{.first_name = "Bart"}));
  children->insert(std::make_pair(2, Person{.first_name = "Lisa"}));
  children->insert(std::make_pair(3, Person{.first_name = "Maggie"}));

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  write_and_read(homer);
}
}  // namespace test_map
