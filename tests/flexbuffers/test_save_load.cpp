#include "test_save_load.hpp"

#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_save_load {

using Age = rfl::Validator<unsigned int,
                           rfl::AllOf<rfl::Minimum<0>, rfl::Maximum<130>>>;

struct Person {
  rfl::Field<"firstName", std::string> first_name;
  rfl::Field<"lastName", std::string> last_name;
  rfl::Field<"birthday", rfl::Timestamp<"%Y-%m-%d">> birthday;
  rfl::Field<"age", Age> age;
  rfl::Field<"email", rfl::Email> email;
  rfl::Field<"children", std::vector<Person>> children;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const auto bart = Person{.first_name = "Bart",
                           .last_name = "Simpson",
                           .birthday = "1987-04-19",
                           .age = 10,
                           .email = "bart@simpson.com",
                           .children = std::vector<Person>()};

  const auto lisa = Person{
      .first_name = "Lisa",
      .last_name = "Simpson",
      .birthday = "1987-04-19",
      .age = 8,
      .email = "lisa@simpson.com",
      .children = rfl::default_value  // same as std::vector<Person>()
  };

  const auto maggie =
      rfl::replace(lisa, rfl::make_field<"firstName">(std::string("Maggie")),
                   rfl::make_field<"email">(std::string("maggie@simpson.com")),
                   rfl::make_field<"age">(0));

  const auto homer1 =
      Person{.first_name = "Homer",
             .last_name = "Simpson",
             .birthday = "1987-04-19",
             .age = 45,
             .email = "homer@simpson.com",
             .children = std::vector<Person>({bart, lisa, maggie})};

  rfl::flexbuf::save("homer.fb", homer1);

  const auto homer2 = rfl::flexbuf::load<Person>("homer.fb").value();

  const auto bytes1 = rfl::flexbuf::write(homer1);
  const auto bytes2 = rfl::flexbuf::write(homer2);

  if (bytes1 != bytes2) {
    std::cout << "Test failed. Content was not identical." << std::endl
              << std::endl;
    return;
  }

  std::cout << "OK" << std::endl << std::endl;
}
}  // namespace test_save_load
