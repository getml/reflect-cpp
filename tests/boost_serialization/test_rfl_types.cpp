#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_rfl_types {

struct PersonRenamed {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  int age;
};

TEST(boost_serialization, test_rename) {
  const auto homer = PersonRenamed{.first_name = "Homer", .age = 45};
  write_and_read_with_json(homer);
}

using FirstName = rfl::Literal<"Homer", "Marge", "Bart", "Lisa", "Maggie">;

struct PersonLiteral {
  FirstName first_name;
  std::vector<PersonLiteral> children;
};

TEST(boost_serialization, test_literal) {
  const auto bart = PersonLiteral{.first_name = FirstName::make<"Bart">()};
  write_and_read(bart);
}

struct Inner {
  rfl::Field<"firstName", std::string> first_name;
  rfl::Field<"age", int> age;
};

struct Outer {
  rfl::Flatten<Inner> person;
  rfl::Field<"employer", std::string> employer;
};

TEST(boost_serialization, test_flatten) {
  const auto employee = Outer{.person = Inner{.first_name = "Homer", .age = 45},
                              .employer = "Mr. Burns"};
  write_and_read_with_json(employee);
}

struct Circle {
  double radius;
};

struct Rectangle {
  double height;
  double width;
};

using Shapes = rfl::TaggedUnion<"shape", Circle, Rectangle>;

TEST(boost_serialization, test_tagged_union) {
  const Shapes r = Rectangle{.height = 10, .width = 5};
  write_and_read_with_json(r);
}

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct PersonValidated {
  std::string first_name;
  Age age;
  rfl::Email email;
};

TEST(boost_serialization, test_validator) {
  const auto homer = PersonValidated{
      .first_name = "Homer", .age = 45, .email = "homer@simpson.com"};
  write_and_read_with_json(homer);
}

using TS = rfl::Timestamp<"%Y-%m-%d">;

struct PersonWithTimestamp {
  std::string first_name;
  TS birthday;
};

TEST(boost_serialization, test_timestamp) {
  const auto bart =
      PersonWithTimestamp{.first_name = "Bart", .birthday = "1987-04-19"};
  write_and_read_with_json(bart);
}

}  // namespace test_rfl_types
