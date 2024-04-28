#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace test_error_messages {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  std::vector<Person> children;
};

TEST(json, test_error_messages) {
  const std::string faulty_string =
      R"({"firstName":"Homer","lastName":12345,"birthday":"04/19/1987"})";

  const auto result = rfl::json::read<Person>(faulty_string);

  const std::string expected = R"(Found 3 errors:
1) Failed to parse field 'lastName': Could not cast to string.
2) Failed to parse field 'birthday': String '04/19/1987' did not match format '%Y-%m-%d'.
3) Field named 'children' not found.)";

  EXPECT_TRUE(result.error() && true);

  EXPECT_EQ(result.error().value().what(), expected);
}
}  // namespace test_error_messages
