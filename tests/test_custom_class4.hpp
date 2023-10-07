#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace tcc4 {

struct Person {
    Person(const std::string& _first_name,
           const rfl::Box<std::string>& _last_name, int _age)
        : first_name_(_first_name),
          last_name_(rfl::make_box<std::string>(*_last_name)),
          age_(_age) {}

    const auto& first_name() const { return first_name_; }

    const auto& last_name() const { return last_name_; }

    auto age() const { return age_; }

   private:
    std::string first_name_;
    rfl::Box<std::string> last_name_;
    int age_;
};

struct PersonImpl {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", rfl::Box<std::string>> last_name;
    rfl::Field<"age", int> age;

    static PersonImpl from_class(const Person& _p) noexcept {
        return PersonImpl{
            .first_name = _p.first_name(),
            .last_name = rfl::make_box<std::string>(*_p.last_name()),
            .age = _p.age()};
    }
};

}  // namespace tcc4

namespace rfl {
namespace parsing {

template <class ReaderType, class WriterType>
struct Parser<ReaderType, WriterType, tcc4::Person>
    : public CustomParser<ReaderType, WriterType, tcc4::Person,
                          tcc4::PersonImpl> {};

}  // namespace parsing
}  // namespace rfl

void test_custom_class4() {
    std::cout << "test_custom_class4" << std::endl;

    const auto bart =
        tcc4::Person("Bart", rfl::make_box<std::string>("Simpson"), 10);

    write_and_read(bart,
                   R"({"firstName":"Bart","lastName":"Simpson","age":10})");
}
