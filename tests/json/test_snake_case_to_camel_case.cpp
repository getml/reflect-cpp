#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_snake_case_to_camel_case {

struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  std::vector<Person> children;
};

TEST(json, test_snake_case_to_camel_case) {
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

  std::cout << rfl::json::write<rfl::SnakeCaseToCamelCase>(homer,
                                                           rfl::json::pretty)
            << std::endl;
}
}  // namespace test_snake_case_to_camel_case
