#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

void test_flatten_tuple() {
  std::cout << "test_flatten_tuple" << std::endl;

  using Person = std::tuple<std::string, rfl::Box<std::string>, int>;

  struct Employee {
    rfl::Flatten<Person> person;
    rfl::Box<std::string> employer;
    float salary;
  };

  const auto employee = Employee{
      .person = Person{"Homer", rfl::make_box<std::string>("Simpson"), 45},
      .employer = rfl::make_box<std::string>("Mr. Burns"),
      .salary = 60000.0};

  write_and_read(employee, R"(["Homer","Simpson",45,"Mr. Burns",60000.0])");
}
