#include <cassert>
#include <rfl.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_flatten {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", rfl::Box<std::string>> last_name;
  rfl::Rename<"age", int> age;
};

struct Employee {
  rfl::Flatten<Person> person;
  rfl::Rename<"employer", rfl::Box<std::string>> employer;
  rfl::Rename<"salary", float> salary;
};

TEST(avro, test_flatten) {
  const auto employee = Employee{
      .person = Person{.first_name = "Homer",
                       .last_name = rfl::make_box<std::string>("Simpson"),
                       .age = 45},
      .employer = rfl::make_box<std::string>("Mr. Burns"),
      .salary = 60000.0};

  write_and_read_with_json(employee);
}
}  // namespace test_flatten
