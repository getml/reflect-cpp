#include <gtest/gtest.h>

#include <cassert>
#include <rfl.hpp>
#include <rfl/env.hpp>
#include <string>

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

TEST(env, test_flatten_anonymous) {
  const auto employee = Employee{
      .person = Person{.first_name = "Homer",
                       .last_name = rfl::make_box<std::string>("Simpson"),
                       .age = 45},
      .employer = rfl::make_box<std::string>("Mr. Burns"),
      .salary = 60000.0};
}

}  // namespace test_flatten_anonymous
