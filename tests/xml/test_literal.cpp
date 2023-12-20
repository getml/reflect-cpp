#include "test_literal.hpp"

#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_literal {

using FirstName = rfl::Literal<"Homer", "Marge", "Bart", "Lisa", "Maggie">;
using LastName = rfl::Literal<"Simpson">;

struct Person {
  rfl::Rename<"firstName", FirstName> first_name;
  rfl::Rename<"lastName", LastName> last_name;
  std::vector<Person> children;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const auto bart = Person{.first_name = FirstName::make<"Bart">()};

  write_and_read(bart);
}
}  // namespace test_literal
