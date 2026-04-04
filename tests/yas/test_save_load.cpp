#include <iostream>
#include <rfl.hpp>
#include <rfl/yas.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_save_load {
struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  rfl::Email email;
  int age;
};

TEST(yas, test_save_load) {
  const auto homer = Person{.first_name = "Homer",
                            .last_name = "Simpson",
                            .email = "homer@simpson.com",
                            .age = 45};

  rfl::yas::save("homer.yas", homer);

  const auto homer2 = rfl::yas::load<Person>("homer.yas").value();

  write_and_read(homer2);
}
}  // namespace test_save_load
