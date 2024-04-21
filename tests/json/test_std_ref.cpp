#include <functional>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>

#include <gtest/gtest.h>

namespace test_std_ref {

struct StdRefStruct {
  std::reference_wrapper<int> a;
};

TEST(json, test_std_ref) {
  int i = 10;
  StdRefStruct struct_ = {.a = std::ref(i)};

  const auto json_string = rfl::json::write(struct_);
  const std::string& expected = R"({"a":10})";
  EXPECT_EQ(json_string, expected) << "Test failed on write. Expected:" << std::endl
              << expected << std::endl
              << "Got: " << std::endl
              << json_string << std::endl
              << std::endl;
}
}  // namespace test_std_ref
