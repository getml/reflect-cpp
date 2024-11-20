#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_no_extra_fields {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
};

TEST(json, test_no_extra_fields) {
  auto homer = rfl::json::read<Person, rfl::NoExtraFields>(
      R"({"first_name":"Homer","last_name":"Simpson","extra_field":0})");

  EXPECT_EQ(homer && true, false);
}
}  // namespace test_no_extra_fields