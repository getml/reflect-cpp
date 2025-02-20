#include <iostream>
#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_attribute_rename {

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

TEST(xml, test_attribute_rename) {
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

  const auto expected = R"(<?xml version="1.0" encoding="UTF-8"?>
<Person firstName="Homer" lastName="Simpson" town="Springfield" birthday="1987-04-19" age="45" email="homer@simpson.com">
    <child firstName="Bart" lastName="Simpson" town="Springfield" birthday="1987-04-19" age="10" email="bart@simpson.com" />
    <child firstName="Lisa" lastName="Simpson" town="Springfield" birthday="1987-04-19" age="8" email="lisa@simpson.com" />
    <child firstName="Maggie" lastName="Simpson" town="Springfield" birthday="1987-04-19" age="0" email="maggie@simpson.com" />
</Person>
)";

  EXPECT_EQ(rfl::xml::write(homer), expected);
}

}  // namespace test_attribute_rename
