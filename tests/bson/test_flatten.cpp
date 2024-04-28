#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_flatten {

struct Person {
  rfl::Field<"firstName", std::string> first_name;
  rfl::Field<"lastName", rfl::Box<std::string>> last_name;
  rfl::Field<"age", int> age;
};

struct Employee {
  rfl::Flatten<Person> person;
  rfl::Field<"employer", rfl::Box<std::string>> employer;
  rfl::Field<"salary", float> salary;
};

TEST(bson, test_flatten) {
  const auto employee = Employee{
      .person = Person{.first_name = "Homer",
                       .last_name = rfl::make_box<std::string>("Simpson"),
                       .age = 45},
      .employer = rfl::make_box<std::string>("Mr. Burns"),
      .salary = 60000.0};

  write_and_read(employee);
}
}  // namespace test_flatten
