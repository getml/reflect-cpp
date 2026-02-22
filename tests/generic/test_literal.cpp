#include <cassert>
#include <rfl.hpp>
#include <type_traits>
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

TEST(generic, test_literal) {
  const auto bart = Person{.first_name = FirstName::make<"Bart">()};

  write_and_read(bart);
}
}  // namespace test_literal

namespace test_literal_explicit {

TEST(regression, literal_constructor_is_explicit) {
  using LitType = rfl::Literal<"a", "b", "c">;
  constexpr bool is_implicit =
      std::is_convertible_v<std::string, LitType>;
  EXPECT_FALSE(is_implicit)
      << "Literal(const std::string&) should be explicit to prevent "
         "unexpected exceptions from implicit conversions";
}

}  // namespace test_literal_explicit
