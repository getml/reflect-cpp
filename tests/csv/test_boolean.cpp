#include <rfl/csv.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_boolean {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::string town = "Springfield";
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  bool is_child;
  Age age;
  rfl::Email email;
};

TEST(csv, test_boolean) {
  const auto people =
      std::vector<Person>({Person{.first_name = "Bart",
                                  .birthday = "1987-04-19",
                                  .is_child = true,
                                  .age = 10,
                                  .email = "bart@simpson.com"},
                           Person{.first_name = "Lisa",
                                  .birthday = "1987-04-19",
                                  .is_child = true,
                                  .age = 8,
                                  .email = "lisa@simpson.com"},
                           Person{.first_name = "Maggie",
                                  .birthday = "1987-04-19",
                                  .is_child = true,
                                  .age = 0,
                                  .email = "maggie@simpson.com"},
                           Person{.first_name = "Homer",
                                  .birthday = "1987-04-19",
                                  .is_child = false,
                                  .age = 45,
                                  .email = "homer@simpson.com"}});

  write_and_read(people);
}
}  // namespace test_boolean
