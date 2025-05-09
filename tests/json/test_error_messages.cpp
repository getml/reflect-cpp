#include <gtest/gtest.h>

#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

namespace test_error_messages {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  rfl::Timestamp<"%Y-%m-%d"> birthday;
  std::vector<Person> children;
};

TEST(json, test_field_error_messages) {
  const std::string faulty_string =
      R"({"firstName":"Homer","lastName":12345,"birthday":"04/19/1987"})";

  const auto result = rfl::json::read<Person>(faulty_string);

  const std::string expected = R"(Found 3 errors:
1) Failed to parse field 'lastName': Could not cast to string.
2) Failed to parse field 'birthday': String '04/19/1987' did not match format '%Y-%m-%d'.
3) Field named 'children' not found.)";

  EXPECT_TRUE(!result.has_value() && true);

  EXPECT_EQ(result.error().what(), expected);
}

TEST(json, test_decode_error_without_exception) {
  // Note using valid field names, but invalid JSON delimiter ';'
  const std::string faulty_string =
      R"({"firstName":"Homer";"lastName":"Simpson";"birthday":"1987-04-19"})";

  rfl::Result<Person> result = rfl::error("result didn't get set");
  EXPECT_NO_THROW({
    result = rfl::json::read<Person>(faulty_string);
  });

  EXPECT_TRUE(!result.has_value() && true);

  EXPECT_EQ(result.error().what(), "Could not parse document");
}

}  // namespace test_error_messages
