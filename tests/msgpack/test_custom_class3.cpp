#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_custom_class3 {

struct Person {
  Person(const std::string& _first_name, const std::string& _last_name,
         const int _age)
      : first_name_(_first_name), last_name_(_last_name), age_(_age) {}

  const auto& first_name() const { return first_name_; }

  const auto& last_name() const { return last_name_; }

  auto age() const { return age_; }

 private:
  std::string first_name_;
  std::string last_name_;
  int age_;
};

struct PersonImpl {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  int age;

  static PersonImpl from_class(const Person& _p) noexcept {
    return PersonImpl{.first_name = _p.first_name(),
                      .last_name = _p.last_name(),
                      .age = _p.age()};
  }

  Person to_class() const { return Person(first_name(), last_name(), age); }
};
}  // namespace test_custom_class3

namespace rfl {
namespace parsing {

template <class ReaderType, class WriterType>
struct Parser<ReaderType, WriterType, test_custom_class3::Person>
    : public CustomParser<ReaderType, WriterType, test_custom_class3::Person,
                          test_custom_class3::PersonImpl> {};

}  // namespace parsing
}  // namespace rfl

namespace test_custom_class3 {

TEST(msgpack, test_custom_class3) {
  const auto bart = Person("Bart", "Simpson", 10);

  write_and_read(bart);
}

}  // namespace test_custom_class3
