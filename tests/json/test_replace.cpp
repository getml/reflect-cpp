#include <cassert>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_replace {

struct Person {
  rfl::Field<"firstName", std::string> first_name;
  rfl::Field<"lastName", std::string> last_name;
  rfl::Field<"children", rfl::Box<std::vector<Person>>> children;
};

TEST(json, test_replace) {
  auto lisa = Person{.first_name = "Lisa",
                     .last_name = "Simpson",
                     .children = rfl::make_box<std::vector<Person>>()};

  auto maggie = rfl::replace(
      std::move(lisa), rfl::make_field<"firstName">(std::string("Maggie")));

  write_and_read(
      maggie, R"({"firstName":"Maggie","lastName":"Simpson","children":[]})");

  auto bart =
      rfl::replace(std::move(maggie),
                   rfl::make_field<&Person::first_name>(std::string("Bart")));

  write_and_read(bart,
                 R"({"firstName":"Bart","lastName":"Simpson","children":[]})");
}
}  // namespace test_replace
