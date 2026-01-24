#ifdef REFLECTCPP_YAML

#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

import rfl;

namespace test_yaml {

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

TEST(modules, test_yaml) {
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

  const auto yaml_str = rfl::yaml::write(homer);
  const auto homer2 = rfl::yaml::read<Person>(yaml_str).value();
  const auto yaml_str2 = rfl::yaml::write(homer2);

  ASSERT_EQ(yaml_str, yaml_str2);
}

}  // namespace test_yaml

#endif  // REFLECTCPP_JSON
