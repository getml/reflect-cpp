#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_strip_field_names {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::string town = "Springfield";
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  Age age;
  rfl::Email email;
  std::vector<Person> children;
};

TEST(json, test_strip_field_names) {
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

  const auto homer =
      Person{.first_name = "Homer",
             .birthday = "1987-04-19",
             .age = 45,
             .email = "homer@simpson.com",
             .children = std::vector<Person>({bart, lisa, maggie})};

  write_and_read<rfl::NoFieldNames>(
      homer,
      R"(["Homer","Simpson","Springfield","1987-04-19",45,"homer@simpson.com",[["Bart","Simpson","Springfield","1987-04-19",10,"bart@simpson.com",[]],["Lisa","Simpson","Springfield","1987-04-19",8,"lisa@simpson.com",[]],["Maggie","Simpson","Springfield","1987-04-19",0,"maggie@simpson.com",[]]]])");
}
}  // namespace test_strip_field_names
