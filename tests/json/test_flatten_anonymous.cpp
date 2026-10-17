#include <cassert>
#include <string>

#include "write_and_read.hpp"
#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


namespace test_flatten_anonymous {

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

TEST(json, test_flatten_anonymous) {
  const auto employee = Employee{
      .person = Person{.first_name = "Homer",
                       .last_name = rfl::make_box<std::string>("Simpson"),
                       .age = 45},
      .employer = rfl::make_box<std::string>("Mr. Burns"),
      .salary = 60000.0};

  write_and_read(
      employee,
      R"({"first_name":"Homer","last_name":"Simpson","age":45,"employer":"Mr. Burns","salary":60000.0})");
}

}  // namespace test_flatten_anonymous
