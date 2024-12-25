#include <gtest/gtest.h>

#include <iostream>
#include <rfl.hpp>
#include <rfl/capnproto.hpp>
#include <string>
#include <vector>

// #include "write_and_read.hpp"

namespace test_tutorial_example {

const std::string PERSON_SCHEMA = R"(
@0xb07899ba441fd7ec;

struct Person {
  firstName @0 :Text;
  lastName @1 :Text;
}
)";

struct Person {
  std::string firstName;
  std::string lastName;
};

TEST(capnproto, test_person) {
  const auto schema =
      rfl::capnproto::Schema<Person>::from_string(PERSON_SCHEMA).value();
  const auto homer = Person{.firstName = "Homer", .lastName = "Simpson"};
  std::cout << rfl::capnproto::write(homer, schema).data() << std::endl;
}
}  // namespace test_tutorial_example
