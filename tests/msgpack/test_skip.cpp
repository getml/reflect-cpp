#include <iostream>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_skip {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  rfl::Skip<std::string> town;
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  Age age;
};

TEST(msgpack, test_skip) {
  const auto homer = Person{.town = "Springfield",
                            .first_name = "Homer",
                            .last_name = "Simpson",
                            .age = 45};

  write_and_read(homer);
}
}  // namespace test_skip
