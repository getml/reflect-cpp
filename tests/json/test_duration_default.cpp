#include <chrono>

#include "write_and_read.hpp"
#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


namespace test_duration_default {

struct TestStruct {
  std::chrono::seconds duration;
};

TEST(json, test_duration_default) {
  const auto test = TestStruct{.duration = std::chrono::seconds(10)};
  write_and_read<rfl::DefaultIfMissing>(
      test, R"({"duration":{"count":10,"unit":"seconds"}})");
}
}  // namespace test_duration_default
