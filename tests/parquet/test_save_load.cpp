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
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  Age age;
  rfl::Email email;
};

TEST(parquet, test_save_load) {
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

  rfl::parquet::save("people.parquet", people1);

  const auto people2 =
      rfl::parquet::load<std::vector<Person>>("people.parquet").value();

  const auto bytes1 = rfl::parquet::write(people1);
  const auto bytes2 = rfl::parquet::write(people2);

  EXPECT_EQ(bytes1, bytes2);
}
}  // namespace test_save_load
