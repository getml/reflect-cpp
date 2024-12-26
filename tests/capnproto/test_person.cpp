#include <gtest/gtest.h>

#include <iostream>
#include <rfl.hpp>
#include <rfl/capnproto.hpp>
#include <string>
#include <vector>

// #include "write_and_read.hpp"

namespace test_tutorial_example {

struct Person {
  std::string first_name;
  std::string last_name;
};

TEST(capnproto, test_person) {
  const auto schema = rfl::capnproto::to_schema<Person>();
  const auto homer1 = Person{.first_name = "Homer", .last_name = "Simpson"};
  const auto serialized1 = rfl::capnproto::write(homer1, schema);
  const auto homer2 = rfl::capnproto::read<Person>(serialized1, schema).value();
  const auto serialized2 = rfl::capnproto::write(homer2, schema);
  EXPECT_EQ(serialized1, serialized2);
}
}  // namespace test_tutorial_example
