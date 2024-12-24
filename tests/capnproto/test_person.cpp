#include <gtest/gtest.h>

#include <iostream>
#include <rfl.hpp>
#include <rfl/capnproto.hpp>
#include <string>
#include <vector>

// #include "write_and_read.hpp"

/// The basic example from the Avro C tutorial.
namespace test_tutorial_example {

const std::string PERSON_SCHEMA = R"(
@0xb07899ba441fd7ec;

struct Person {
  first_name @0 :Text;
  last_name @2 :Text;
};
)";

struct Person {
  std::string first_name;
  std::string last_name;
};

TEST(capnproto, test_person) {
  const auto schema =
      rfl::capnproto::Schema<Person>::from_string(PERSON_SCHEMA).value();
  std::cout << schema.str() << std::endl;
  const auto homer = Person{.first_name = "Homer", .last_name = "Simpson"};
  std::cout << rfl::capnproto::write(homer, schema).data() << std::endl;
}
}  // namespace test_tutorial_example
