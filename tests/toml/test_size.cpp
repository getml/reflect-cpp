#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_size {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  rfl::Validator<std::vector<Person>,
                 rfl::Size<rfl::AnyOf<rfl::EqualTo<0>, rfl::EqualTo<3>>>>
      children;
};

TEST(toml, test_size) { 
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

  write_and_read(homer);
}
}  // namespace test_size
