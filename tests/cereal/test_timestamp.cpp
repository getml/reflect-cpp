#include <iostream>
#include <string>
#include <vector>

#include "write_and_read.hpp"
import rfl;
import rfl.cereal;


namespace test_timestamp {

struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Timestamp<"%Y-%m-%d"> birthday;
};

TEST(cereal, test_timestamp) {
  const auto person = Person{.first_name = "Homer",
                             .last_name = "Simpson",
                             .birthday = "1987-04-19"};

  write_and_read(person);
}
}  // namespace test_timestamp
