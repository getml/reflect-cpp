#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_custom_class1 {

struct Person {
  struct PersonImpl {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name = "Simpson";
    std::vector<Person> children;
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
  std::cout << "test_custom_class1" << std::endl;

  const auto bart = Person("Bart");

  write_and_read(bart,
                 R"({"firstName":"Bart","lastName":"Simpson","children":[]})");
}
}  // namespace test_custom_class1
