#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_anonymous_fields {

using Age = rfl::Validator<unsigned int,
                           rfl::AllOf<rfl::Minimum<0>, rfl::Maximum<130>>>;

struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  Age age;
  rfl::Email email;
  std::vector<Person> children;
};

TEST(json, test_anonymous_fields) {
  const auto bart = Person{.first_name = "Bart",
                           .last_name = "Simpson",
                           .birthday = "1987-04-19",
                           .age = 10,
                           .email = "bart@simpson.com"};

  const auto lisa = Person{.first_name = "Lisa",
                           .last_name = "Simpson",
                           .birthday = "1987-04-19",
                           .age = 8,
                           .email = "lisa@simpson.com"};

  const auto maggie = Person{.first_name = "Maggie",
                             .last_name = "Simpson",
                             .birthday = "1987-04-19",
                             .age = 0,
                             .email = "maggie@simpson.com"};

  const auto homer =
      Person{.first_name = "Homer",
             .last_name = "Simpson",
             .birthday = "1987-04-19",
             .age = 45,
             .email = "homer@simpson.com",
             .children = std::vector<Person>({bart, lisa, maggie})};

  write_and_read(
      homer,
      R"({"first_name":"Homer","last_name":"Simpson","birthday":"1987-04-19","age":45,"email":"homer@simpson.com","children":[{"first_name":"Bart","last_name":"Simpson","birthday":"1987-04-19","age":10,"email":"bart@simpson.com","children":[]},{"first_name":"Lisa","last_name":"Simpson","birthday":"1987-04-19","age":8,"email":"lisa@simpson.com","children":[]},{"first_name":"Maggie","last_name":"Simpson","birthday":"1987-04-19","age":0,"email":"maggie@simpson.com","children":[]}]})");
}
}  // namespace test_anonymous_fields
