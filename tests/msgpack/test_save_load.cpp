#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/msgpack.hpp>
#include <source_location>
#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace test_save_load {

using Age = rfl::Validator<unsigned int,
                           rfl::AllOf<rfl::Minimum<0>, rfl::Maximum<130>>>;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  Age age;
  rfl::Email email;
  std::vector<Person> children;
};

TEST(msgpack, test_save_load) { 
  const auto bart = Person{.first_name = "Bart",
                           .last_name = "Simpson",
                           .birthday = "1987-04-19",
                           .age = 10,
                           .email = "bart@simpson.com",
                           .children = std::vector<Person>()};

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

  const auto homer1 =
      Person{.first_name = "Homer",
             .last_name = "Simpson",
             .birthday = "1987-04-19",
             .age = 45,
             .email = "homer@simpson.com",
             .children = std::vector<Person>({bart, lisa, maggie})};

  rfl::msgpack::save("homer.msgpack", homer1);

  const auto homer2 = rfl::msgpack::load<Person>("homer.msgpack").value();

  const auto string1 = rfl::msgpack::write(homer1);
  const auto string2 = rfl::msgpack::write(homer2);

  EXPECT_EQ(string1, string2);
}
}  // namespace test_save_load
