#include <rfl/csv.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_flatten {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Address {
  std::string street;
  std::string city;
};

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::string town = "Springfield";
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  Age age;
  rfl::Email email;
  rfl::Flatten<Address> address;
};

TEST(csv, test_flatten) {
  const auto address =
      Address{.street = "Evergreen Terrace", .city = "Springfield"};

  const auto people = std::vector<Person>({Person{.first_name = "Bart",
                                                  .birthday = "1987-04-19",
                                                  .age = 10,
                                                  .email = "bart@simpson.com",
                                                  .address = address},
                                           Person{.first_name = "Lisa",
                                                  .birthday = "1987-04-19",
                                                  .age = 8,
                                                  .email = "lisa@simpson.com",
                                                  .address = address},
                                           Person{.first_name = "Maggie",
                                                  .birthday = "1987-04-19",
                                                  .age = 0,
                                                  .email = "maggie@simpson.com",
                                                  .address = address},
                                           Person{.first_name = "Homer",
                                                  .birthday = "1987-04-19",
                                                  .age = 45,
                                                  .email = "homer@simpson.com",
                                                  .address = address}});

  write_and_read(people);
}
}  // namespace test_flatten
