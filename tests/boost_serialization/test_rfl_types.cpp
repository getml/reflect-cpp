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
  const auto employee =
      Outer{.person = Inner{.first_name = "Homer", .age = 45},
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

}  // namespace test_rfl_types
