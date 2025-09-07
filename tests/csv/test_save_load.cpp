#include <gtest/gtest.h>

#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/csv.hpp>
#include <string>
#include <vector>

namespace test_save_load {

using Age =
    rfl::Validator<int64_t, rfl::AllOf<rfl::Minimum<0>, rfl::Maximum<130>>>;

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::string town = "Springfield";
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  Age age;
  rfl::Email email;
};

TEST(csv, test_save_load) {
  const auto people1 =
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

  const auto settings = rfl::csv::Settings{}.with_delimiter(';');

  rfl::csv::save("people.csv", people1, settings);

  const auto people2 =
      rfl::csv::load<std::vector<Person>>("people.csv", settings).value();

  const auto bytes1 = rfl::csv::write(people1);
  const auto bytes2 = rfl::csv::write(people2);

  EXPECT_EQ(bytes1, bytes2);
}
}  // namespace test_save_load
