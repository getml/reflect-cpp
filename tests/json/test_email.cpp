#include "test_email.hpp"

#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_email {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  rfl::Email email;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const auto homer = Person{.first_name = "Homer",
                            .last_name = "Simpson",
                            .email = "homer@simpson.com"};

  write_and_read(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","email":"homer@simpson.com"})");
}
}  // namespace test_email
