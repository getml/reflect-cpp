#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_literal {

using FirstName = rfl::Literal<"Homer", "Marge", "Bart", "Lisa", "Maggie">;
using LastName = rfl::Literal<"Simpson">;

struct Person {
  rfl::Field<"firstName", FirstName> first_name;
  rfl::Field<"lastName", LastName> last_name = rfl::default_value;
  rfl::Field<"children", std::vector<Person>> children = rfl::default_value;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const auto bart = Person{.first_name = FirstName::make<"Bart">()};

  write_and_read(bart);
}
}  // namespace test_literal
