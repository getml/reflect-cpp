#include <memory>
#include <rfl/csv.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_shared_ptr {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::string town = "Springfield";
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  Age age;
  std::shared_ptr<rfl::Email> email;
};

TEST(csv, test_shared_ptr) {
  auto people = std::vector<Person>();
  people.emplace_back(
      Person{.first_name = "Bart",
             .birthday = "1987-04-19",
             .age = 10,
             .email = std::make_shared<rfl::Email>("bart@simpson.com")});
  people.emplace_back(
      Person{.first_name = "Lisa",
             .birthday = "1987-04-19",
             .age = 8,
             .email = std::make_shared<rfl::Email>("lisa@simpson.com")});
  people.emplace_back(
      Person{.first_name = "Maggie", .birthday = "1987-04-19", .age = 0});
  people.emplace_back(
      Person{.first_name = "Homer",
             .birthday = "1987-04-19",
             .age = 45,
             .email = std::make_shared<rfl::Email>("homer@simpson.com")});

  write_and_read(people);
}
}  // namespace test_shared_ptr
