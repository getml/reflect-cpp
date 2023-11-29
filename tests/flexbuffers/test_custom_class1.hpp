#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_custom_class1 {

struct Person {
  struct PersonImpl {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name = "Simpson";
    rfl::Field<"children", std::vector<Person>> children = rfl::default_value;
  };

  using ReflectionType = PersonImpl;

  Person(const PersonImpl& _impl) : impl(_impl) {}

  Person(const std::string& _first_name)
      : impl(PersonImpl{.first_name = _first_name}) {}

  const ReflectionType& reflection() const { return impl; };

 private:
  PersonImpl impl;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const auto bart = Person("Bart");

  write_and_read(bart);
}
}  // namespace test_custom_class1
