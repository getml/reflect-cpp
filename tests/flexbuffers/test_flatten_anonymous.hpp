#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

void test_flatten_anonymous() {
  std::cout << "test_flatten_anonymous" << std::endl;

  struct Person {
    std::string first_name;
    rfl::Box<std::string> last_name;
    int age;
  };

  struct Employee {
    rfl::Flatten<Person> person;
    rfl::Box<std::string> employer;
    float salary;
  };

  const auto employee = Employee{
      .person = Person{.first_name = "Homer",
                       .last_name = rfl::make_box<std::string>("Simpson"),
                       .age = 45},
      .employer = rfl::make_box<std::string>("Mr. Burns"),
      .salary = 60000.0};

  write_and_read(employee);
}
