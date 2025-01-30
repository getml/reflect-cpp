#include <iostream>
#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_readme_example3 {

struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  std::vector<Person> children;
};

TEST(avro, test_readme_example3) {
  const auto bart = Person{.first_name = "Bart", .birthday = "1987-04-19"};

  const auto lisa = Person{.first_name = "Lisa", .birthday = "1987-04-19"};

  const auto maggie = Person{.first_name = "Maggie", .birthday = "1987-04-19"};

  const auto homer =
      Person{.first_name = "Homer",
             .birthday = "1987-04-19",
             .children = std::vector<Person>({bart, lisa, maggie})};

  write_and_read(homer);
}
}  // namespace test_readme_example3
