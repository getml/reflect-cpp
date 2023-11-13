#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace tcc2 {

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
  rfl::Field<"firstName", std::string> first_name;
  rfl::Field<"lastName", std::string> last_name;
  rfl::Field<"age", int> age;

  static PersonImpl from_class(const Person& _p) noexcept {
    return PersonImpl{.first_name = _p.first_name(),
                      .last_name = _p.last_name(),
                      .age = _p.age()};
  }

  Person to_class() const { return Person(first_name(), last_name(), age()); }
};

}  // namespace tcc2

namespace rfl {
namespace parsing {

template <class ReaderType, class WriterType>
struct Parser<ReaderType, WriterType, tcc2::Person>
    : public CustomParser<ReaderType, WriterType, tcc2::Person,
                          tcc2::PersonImpl> {};

}  // namespace parsing
}  // namespace rfl

void test_custom_class2() {
  std::cout << "test_custom_class2" << std::endl;

  const auto bart = tcc2::Person("Bart", "Simpson", 10);

  write_and_read(bart);
}
