#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

void test_any_of() {
  std::cout << "test_any_of" << std::endl;

  using Age = rfl::Validator<
      unsigned int,
      rfl::AnyOf<rfl::AllOf<rfl::Minimum<0.>, rfl::Maximum<10.>>,
                 rfl::AllOf<rfl::Minimum<40.>, rfl::Maximum<130.>>>>;

  struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name;
    rfl::Field<"age", Age> age;
  };

  const auto homer =
      Person{.first_name = "Homer", .last_name = "Simpson", .age = 45};

  write_and_read(homer,
                 R"({"firstName":"Homer","lastName":"Simpson","age":45})");
}
