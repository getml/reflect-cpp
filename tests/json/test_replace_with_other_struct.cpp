#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_replace_with_other_struct {

struct Person {
  rfl::Field<"firstName", std::string> first_name;
  rfl::Field<"lastName", std::string> last_name;
  rfl::Field<"age", int> age;
};

struct Employee {
  rfl::Flatten<Person> person;
  rfl::Field<"employer", std::string> employer;
  rfl::Field<"salary", float> salary;
};

TEST(json, test_replace_with_other_struct) {
  const auto employee = Employee{
      .person =
          Person{.first_name = "Homer", .last_name = "Simpson", .age = 45},
      .employer = std::string("Mr. Burns"),
      .salary = 60000.0};

  const auto carl = Person{.first_name = "Carl", .last_name = "", .age = 45};

  const auto employee2 = rfl::replace(employee, carl);

  write_and_read(
      employee2,
      R"({"firstName":"Carl","lastName":"","age":45,"employer":"Mr. Burns","salary":60000.0})");
}
}  // namespace test_replace_with_other_struct
