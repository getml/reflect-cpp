#include <gtest/gtest.h>

#include <rfl.hpp>
#include <rfl/env.hpp>
#include <string>
#include <vector>

namespace test_readme_example {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::string town = "Springfield";
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  Age age;
  rfl::Email email;
  std::vector<Person> children;
};

TEST(env, test_readme_example) {
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

  rfl::env::write(homer);

  ASSERT_EQ(std::getenv("FIRST_NAME"), std::string("Homer"));
  ASSERT_EQ(std::getenv("LAST_NAME"), std::string("Simpson"));
  ASSERT_EQ(std::getenv("TOWN"), std::string("Springfield"));
  ASSERT_EQ(std::getenv("BIRTHDAY"), std::string("1987-04-19"));
  ASSERT_EQ(std::getenv("AGE"), std::string("45"));
  ASSERT_EQ(std::getenv("EMAIL"), std::string("homer@simpson.com"));

  ASSERT_EQ(std::getenv("CHILDREN_0_FIRST_NAME"), std::string("Bart"));
  ASSERT_EQ(std::getenv("CHILDREN_0_LAST_NAME"), std::string("Simpson"));
  ASSERT_EQ(std::getenv("CHILDREN_0_TOWN"), std::string("Springfield"));
  ASSERT_EQ(std::getenv("CHILDREN_0_BIRTHDAY"), std::string("1987-04-19"));
  ASSERT_EQ(std::getenv("CHILDREN_0_AGE"), std::string("10"));
}

}  // namespace test_readme_example
