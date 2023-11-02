#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

// Makes sure that the example in the README works as expected.
// It would be embarrassing if it didn't...
void test_size() {
  std::cout << "test_size" << std::endl;

  struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name;
    rfl::Field<"birthday", rfl::Timestamp<"%Y-%m-%d">> birthday;
    rfl::Field<"children",
               rfl::Validator<
                   std::vector<Person>,
                   rfl::Size<rfl::AnyOf<rfl::EqualTo<0.>, rfl::EqualTo<3.>>>>>
        children;
  };

  const auto bart = Person{.first_name = "Bart",
                           .last_name = "Simpson",
                           .birthday = "1987-04-19",
                           .children = std::vector<Person>()};

  const auto lisa = Person{
      .first_name = "Lisa",
      .last_name = "Simpson",
      .birthday = "1987-04-19",
      .children = rfl::default_value  // same as std::vector<Person>()
  };

  // Returns a deep copy of the original object,
  // replacing first_name.
  const auto maggie =
      rfl::replace(lisa, rfl::make_field<"firstName">(std::string("Maggie")));

  const auto homer =
      Person{.first_name = "Homer",
             .last_name = "Simpson",
             .birthday = "1987-04-19",
             .children = std::vector<Person>({bart, lisa, maggie})};

  write_and_read(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","birthday":"1987-04-19","children":[{"firstName":"Bart","lastName":"Simpson","birthday":"1987-04-19","children":[]},{"firstName":"Lisa","lastName":"Simpson","birthday":"1987-04-19","children":[]},{"firstName":"Maggie","lastName":"Simpson","birthday":"1987-04-19","children":[]}]})");
}
