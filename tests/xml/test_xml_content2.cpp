#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_xml_content2 {

struct Person {
  std::string xml_content;
  rfl::Attribute<std::string> town = "Springfield";
  rfl::Attribute<rfl::Timestamp<"%Y-%m-%d">> birthday;
  rfl::Attribute<rfl::Email> email;
  std::vector<Person> child;
};

TEST(xml, test_xml_content2) {
  const auto bart = Person{.xml_content = "Bart Simpson",
                           .birthday = "1987-04-19",
                           .email = "bart@simpson.com"};

  const auto lisa = Person{.xml_content = "Lisa Simpson",
                           .birthday = "1987-04-19",
                           .email = "lisa@simpson.com"};

  const auto maggie = Person{.xml_content = "Maggie Simpson",
                             .birthday = "1987-04-19",
                             .email = "maggie@simpson.com"};

  const auto homer = Person{.xml_content = "Homer Simpson",
                            .birthday = "1987-04-19",
                            .email = "homer@simpson.com",
                            .child = std::vector<Person>({bart, lisa, maggie})};

  write_and_read<"", rfl::SnakeCaseToCamelCase>(homer);
}
}  // namespace test_xml_content2
