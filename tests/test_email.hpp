#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

void test_email() {
  std::cout << "test_email" << std::endl;

  struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name;
    rfl::Field<"email", rfl::Email> email;
  };

  const auto homer = Person{.first_name = "Homer",
                            .last_name = "Simpson",
                            .email = "homer@simpson.com"};

  write_and_read(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","email":"homer@simpson.com"})");
}
