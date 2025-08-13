#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
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

TEST(json, test_literal) {
  static_assert(!FirstName::template contains_any<LastName>(),
                "Must be false.");
  static_assert(FirstName::template contains_any<FirstName>(), "Must be true.");
  static_assert(FirstName::template contains_all<FirstName>(), "Must be true.");

  const auto bart = Person{.first_name = FirstName::make<"Bart">()};

  write_and_read(bart,
                 R"({"firstName":"Bart","lastName":"Simpson","children":[]})");
}
}  // namespace test_literal
