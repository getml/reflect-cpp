#include <gtest/gtest.h>

#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/parquet.hpp>
#include <string>
#include <vector>

namespace test_save_load {

using Age = rfl::Validator<unsigned int,
                           rfl::AllOf<rfl::Minimum<0>, rfl::Maximum<130>>>;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::string town = "Springfield";
  // rfl::Timestamp<"%Y-%m-%d"> birthday;
  std::string birthday;
  Age age;
  rfl::Email email;
};

TEST(parquet, test_save_load) {
  const auto people =
      std::vector<Person>({Person{.first_name = "Bart",
                                  .birthday = "1987-04-19",
                                  .age = 10,
                                  .email = "bart@simpson.com"},
                           Person{.first_name = "Lisa",
                                  .birthday = "1987-04-19",
                                  .age = 8,
                                  .email = "lisa@simpson.com"},
                           Person{.first_name = "Maggie",
                                  .birthday = "1987-04-19",
                                  .age = 0,
                                  .email = "maggie@simpson.com"},
                           Person{.first_name = "Homer",
                                  .birthday = "1987-04-19",
                                  .age = 45,
                                  .email = "homer@simpson.com"}});

  rfl::parquet::save("people.parquet", people);

  /*const auto homer2 = rfl::toml::load<Person>("homer.toml").value();

  const auto string1 = rfl::toml::write(homer1);
  const auto string2 = rfl::toml::write(homer2);

  EXPECT_EQ(string1, string2);*/
}
}  // namespace test_save_load
