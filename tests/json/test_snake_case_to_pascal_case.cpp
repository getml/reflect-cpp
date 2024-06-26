#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_snake_case_to_pascal_case {

struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  std::vector<Person> children;
};

TEST(json, test_snake_case_to_pascal_case) {
  const auto bart = Person{
      .first_name = "Bart", .last_name = "Simpson", .birthday = "1987-04-19"};

  const auto lisa = Person{
      .first_name = "Lisa", .last_name = "Simpson", .birthday = "1987-04-19"};

  const auto maggie = Person{
      .first_name = "Maggie", .last_name = "Simpson", .birthday = "1987-04-19"};

  const auto homer =
      Person{.first_name = "Homer",
             .last_name = "Simpson",
             .birthday = "1987-04-19",
             .children = std::vector<Person>({bart, lisa, maggie})};

  write_and_read<rfl::SnakeCaseToPascalCase>(
      homer,
      R"({"FirstName":"Homer","LastName":"Simpson","Birthday":"1987-04-19","Children":[{"FirstName":"Bart","LastName":"Simpson","Birthday":"1987-04-19","Children":[]},{"FirstName":"Lisa","LastName":"Simpson","Birthday":"1987-04-19","Children":[]},{"FirstName":"Maggie","LastName":"Simpson","Birthday":"1987-04-19","Children":[]}]})");
}
}  // namespace test_snake_case_to_pascal_case
