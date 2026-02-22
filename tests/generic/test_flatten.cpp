#include <cassert>
#include <rfl.hpp>
#include <string>

#include "test_helpers.hpp"

#include "write_and_read.hpp"

namespace test_flatten {

struct Person {
  rfl::Field<"firstName", std::string> first_name;
  rfl::Field<"lastName", rfl::Box<std::string>> last_name;
  rfl::Field<"age", int> age;
};

struct Employee {
  rfl::Flatten<Person> person;
  rfl::Field<"employer", rfl::Box<std::string>> employer;
  rfl::Field<"salary", float> salary;
};

TEST(generic, test_flatten) {
  const auto employee = Employee{
      .person = Person{.first_name = "Homer",
                       .last_name = rfl::make_box<std::string>("Simpson"),
                       .age = 45},
      .employer = rfl::make_box<std::string>("Mr. Burns"),
      .salary = 60000.0};

  write_and_read(employee);
}
}  // namespace test_flatten

namespace test_flatten_cross_move {

using test_helpers::FlatBase;
using test_helpers::FlatDerived;

TEST(regression, flatten_cross_type_move_does_not_copy) {
  auto source = rfl::Flatten<FlatDerived>(FlatDerived{});
  source.get().copies = 0;
  source.get().moves = 0;

  auto dest = rfl::Flatten<FlatBase>(std::move(source));
  EXPECT_TRUE(dest.get().from_rvalue)
      << "Flatten cross-type move constructor should use rvalue conversion, "
         "but get() returns lvalue ref, so lvalue conversion is used instead";
}

}  // namespace test_flatten_cross_move

namespace test_flatten_universal_ref_ctor {

using test_helpers::FlatBase;
using test_helpers::FlatDerived;

TEST(regression, flatten_universal_ref_ctor_forwards_rvalue) {
  auto val = FlatDerived{};
  val.copies = 0;
  val.moves = 0;

  auto flat = rfl::Flatten<FlatBase>(std::move(val));
  EXPECT_TRUE(flat.get().from_rvalue)
      << "Flatten(U&&) should forward rvalue to Type's converting "
         "move ctor, but value_(_value) without std::forward causes a copy";
}

}  // namespace test_flatten_universal_ref_ctor
