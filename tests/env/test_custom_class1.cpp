#include <gtest/gtest.h>

#include <cassert>
#include <rfl.hpp>
#include <rfl/env.hpp>
#include <string>
#include <vector>

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

TEST(env, test_custom_class1) { const auto bart = Person("Bart"); }
}  // namespace test_custom_class1
