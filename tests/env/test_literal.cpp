#include <gtest/gtest.h>

#include <cassert>
#include <rfl.hpp>
#include <rfl/env.hpp>
#include <vector>

namespace test_literal {

using FirstName = rfl::Literal<"Homer", "Marge", "Bart", "Lisa", "Maggie">;
using LastName = rfl::Literal<"Simpson">;

struct Person {
  rfl::Rename<"firstName", FirstName> first_name;
  rfl::Rename<"lastName", LastName> last_name;
  std::vector<Person> children;
};

TEST(env, test_literal) {
  const auto bart = Person{.first_name = FirstName::make<"Bart">()};
}
}  // namespace test_literal
