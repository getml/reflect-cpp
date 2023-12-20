#include "test_attributes.hpp"

#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_attributes {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  rfl::Rename<"firstName", rfl::Attribute<std::string>> first_name;
  rfl::Rename<"lastName", rfl::Attribute<std::string>> last_name = "Simpson";
  rfl::Attribute<std::string> town = "Springfield";
  rfl::Attribute<rfl::Timestamp<"%Y-%m-%d">> birthday;
  rfl::Attribute<Age> age;
  rfl::Attribute<rfl::Email> email;
  std::vector<Person> child;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const auto bart = Person{.first_name = "Bart",
                           .birthday = "1987-04-19",
                           .age = 10,
                           .email = "bart@simpson.com"};

  const auto lisa = Person{.first_name = "Lisa",
                           .birthday = "1987-04-19",
                           .age = 8,
                           .email = "lisa@simpson.com"};

  const auto maggie = Person{.first_name = "Maggie",
                             .birthday = "1987-04-19",
                             .age = 0,
                             .email = "maggie@simpson.com"};

  const auto homer = Person{.first_name = "Homer",
                            .birthday = "1987-04-19",
                            .age = 45,
                            .email = "homer@simpson.com",
                            .child = std::vector<Person>({bart, lisa, maggie})};

  write_and_read(homer);
}
}  // namespace test_attributes
