#include <chrono>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <vector>

#include "write_and_read.hpp"

namespace test_duration_conversion {

struct TestStruct1 {
  std::chrono::minutes duration;
};

struct TestStruct2 {
  std::chrono::seconds duration;
};

TEST(json, test_duration_conversion) {
  const auto test1 = TestStruct1{.duration = std::chrono::minutes(10)};
  const auto test2 =
      rfl::json::read<TestStruct2>(rfl::json::write(test1)).value();
  write_and_read(test2, R"({"duration":{"count":600,"unit":"seconds"}})");
}
}  // namespace test_duration_conversion
