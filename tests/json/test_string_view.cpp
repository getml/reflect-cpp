#include <gtest/gtest.h>

#include <functional>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string_view>

namespace test_string_view {

struct TestStruct {
  std::string_view name;
};

TEST(json, test_string_view) {
  const std::string name = "Homer Simpson";
  const auto test_struct = TestStruct{.name = name};

  const auto json_string = rfl::json::write(test_struct);
  const std::string expected = R"({"name":"Homer Simpson"})";
  EXPECT_EQ(json_string, expected)
      << "Test failed on write. Expected:" << std::endl
      << expected << std::endl
      << "Got: " << std::endl
      << json_string << std::endl
      << std::endl;
}
}  // namespace test_string_view
