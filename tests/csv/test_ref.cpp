#include <rfl/csv.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_ref {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::string town = "Springfield";
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  Age age;
  rfl::Ref<rfl::Email> email;
};

TEST(csv, test_ref) {
  auto people = std::vector<Person>();
  people.emplace_back(
      Person{.first_name = "Bart",
             .birthday = "1987-04-19",
             .age = 10,
             .email = rfl::make_ref<rfl::Email>("bart@simpson.com")});
  people.emplace_back(
      Person{.first_name = "Lisa",
             .birthday = "1987-04-19",
             .age = 8,
             .email = rfl::make_ref<rfl::Email>("lisa@simpson.com")});
  people.emplace_back(
      Person{.first_name = "Maggie",
             .birthday = "1987-04-19",
             .age = 0,
             .email = rfl::make_ref<rfl::Email>("maggie@simpson.com")});
  people.emplace_back(
      Person{.first_name = "Homer",
             .birthday = "1987-04-19",
             .age = 45,
             .email = rfl::make_ref<rfl::Email>("homer@simpson.com")});

  write_and_read(people);
}
}  // namespace test_ref
