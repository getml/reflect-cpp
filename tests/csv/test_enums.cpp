#include <rfl/csv.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_enums {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

enum class FirstName { Bart, Lisa, Maggie, Homer };

struct Person {
  rfl::Rename<"firstName", FirstName> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::string town = "Springfield";
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  Age age;
  rfl::Email email;
};

TEST(csv, test_enums) {
  const auto people =
      std::vector<Person>({Person{.first_name = FirstName::Bart,
                                  .birthday = "1987-04-19",
                                  .age = 10,
                                  .email = "bart@simpson.com"},
                           Person{.first_name = FirstName::Lisa,
                                  .birthday = "1987-04-19",
                                  .age = 8,
                                  .email = "lisa@simpson.com"},
                           Person{.first_name = FirstName::Lisa,
                                  .birthday = "1987-04-19",
                                  .age = 0,
                                  .email = "maggie@simpson.com"},
                           Person{.first_name = FirstName::Homer,
                                  .birthday = "1987-04-19",
                                  .age = 45,
                                  .email = "homer@simpson.com"}});

  write_and_read(people);
}
}  // namespace test_enums
