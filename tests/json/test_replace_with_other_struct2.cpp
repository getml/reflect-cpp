#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_replace_with_other_struct2 {

struct Person {
  std::string first_name;
  std::string last_name;
  int age;
};

struct Employee {
  rfl::Flatten<Person> person;
  std::string employer;
  float salary;
};

TEST(json, test_replace_with_other_struct2) {
  const auto employee = Employee{
      .person =
          Person{.first_name = "Homer", .last_name = "Simpson", .age = 45},
      .employer = std::string("Mr. Burns"),
      .salary = 60000.0};

  const auto carl = Person{.first_name = "Carl", .last_name = "", .age = 45};

  const auto employee2 = rfl::replace(employee, carl);

  write_and_read(
      employee2,
      R"({"first_name":"Carl","last_name":"","age":45,"employer":"Mr. Burns","salary":60000.0})");
}
}  // namespace test_replace_with_other_struct2
