#include <cassert>
#include <vector>

#include "write_and_read.hpp"
import rfl.capnproto;
import rfl;
import rfl.json;


namespace test_literal {

using FirstName = rfl::Literal<"Homer", "Marge", "Bart", "Lisa", "Maggie">;
using LastName = rfl::Literal<"Simpson">;

struct Person {
  rfl::Rename<"firstName", FirstName> first_name;
  rfl::Rename<"lastName", LastName> last_name;
  std::vector<Person> children;
};

TEST(capnproto, test_literal) {
  const auto bart = Person{.first_name = FirstName::make<"Bart">()};

  write_and_read(bart);
}
}  // namespace test_literal
