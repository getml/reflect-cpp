#include <optional>
#include <rfl/csv.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_optionals {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::string town = "Springfield";
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  Age age;
  std::optional<rfl::Email> email;
};

TEST(csv, test_optionals) {
  const auto people = std::vector<Person>(
      {Person{.first_name = "Bart",
              .birthday = "1987-04-19",
              .age = 10,
              .email = "bart@simpson.com"},
       Person{.first_name = "Lisa",
              .birthday = "1987-04-19",
              .age = 8,
              .email = "lisa@simpson.com"},
       Person{.first_name = "Maggie", .birthday = "1987-04-19", .age = 0},
       Person{.first_name = "Homer",
              .birthday = "1987-04-19",
              .age = 45,
              .email = "homer@simpson.com"}});

  write_and_read(people);
}
}  // namespace test_optionals
