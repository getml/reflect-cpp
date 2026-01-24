#ifdef REFLECTCPP_FLEXBUFFERS

#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

import rfl;

namespace test_flexbuffers {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::string town = "Springfield";
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  Age age;
  rfl::Email email;
  std::vector<Person> children;
};

TEST(modules, test_flexbuffers) {
  const auto bart = Person{.first_name = "Bart",
                           .birthday = "1987-04-19",
                           .age = 10,
                           .email = "bart@simpson.com"};

  const auto lisa = Person{.first_name = "Lisa",
                           .birthday = "1987-04-19",
                           .age = 8,
                           .email = "lisa@simpson.com"};

  const auto maggie = Person{.first_name = "Maggie",
                             .birthday = "1987-04-19",
                             .age = 0,
                             .email = "maggie@simpson.com"};

  const auto homer =
      Person{.first_name = "Homer",
             .birthday = "1987-04-19",
             .age = 45,
             .email = "homer@simpson.com",
             .children = std::vector<Person>({bart, lisa, maggie})};

  const auto flexbuffers_bytes = rfl::flexbuf::write(homer);
  const auto homer2 = rfl::flexbuf::read<Person>(flexbuffers_bytes).value();
  const auto flexbuffers_bytes2 = rfl::flexbuf::write(homer2);

  ASSERT_EQ(flexbuffers_bytes, flexbuffers_bytes2);
}

}  // namespace test_flexbuffers

#endif  // REFLECTCPP_MSGPACK
