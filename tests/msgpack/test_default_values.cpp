#include "test_default_values.hpp"

#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_default_values {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::vector<Person> children;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const auto bart = Person{.first_name = "Bart"};
  const auto lisa = Person{.first_name = "Lisa"};
  const auto maggie = Person{.first_name = "Maggie"};
  const auto homer =
      Person{.first_name = "Homer",
             .children = std::vector<Person>({bart, lisa, maggie})};

  write_and_read(homer);
}
}  // namespace test_default_values
