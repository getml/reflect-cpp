#include "test_readme_example.hpp"

#include <iostream>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_readme_example {

using Age = rfl::Validator<unsigned int,
                           rfl::AllOf<rfl::Minimum<0>, rfl::Maximum<130>>>;

struct Person {
  rfl::Field<"firstName", std::string> first_name;
  rfl::Field<"lastName", std::string> last_name;
  rfl::Field<"birthday", rfl::Timestamp<"%Y-%m-%d">> birthday;
  rfl::Field<"age", Age> age;
  rfl::Field<"email", rfl::Email> email;
  rfl::Field<"children", std::vector<Person>> children;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const auto bart = Person{.first_name = "Bart",
                           .last_name = "Simpson",
                           .birthday = "1987-04-19",
                           .age = 10,
                           .email = "bart@simpson.com",
                           .children = std::vector<Person>()};

  const auto lisa = Person{
      .first_name = "Lisa",
      .last_name = "Simpson",
      .birthday = "1987-04-19",
      .age = 8,
      .email = "lisa@simpson.com",
      .children = rfl::default_value  // same as std::vector<Person>()
  };

  const auto maggie =
      rfl::replace(lisa, rfl::make_field<"firstName">(std::string("Maggie")),
                   rfl::make_field<"email">(std::string("maggie@simpson.com")),
                   rfl::make_field<"age">(0));

  const auto homer =
      Person{.first_name = "Homer",
             .last_name = "Simpson",
             .birthday = "1987-04-19",
             .age = 45,
             .email = "homer@simpson.com",
             .children = std::vector<Person>({bart, lisa, maggie})};

  write_and_read(homer);
}
}  // namespace test_readme_example
