#include <cassert>
#include <chrono>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_chrono_duration {

struct TestStruct {
  std::chrono::milliseconds ms_duration;

  // Not std::chrono::seconds, but should be treated as such.
  std::chrono::duration<int, std::ratio<1>> custom_duration;
};

TEST(json, test_chrono_duration) {
  const auto test = TestStruct{
      .ms_duration = std::chrono::milliseconds(1500),
      .custom_duration = std::chrono::duration<int, std::ratio<1>>(10),
  };

  write_and_read(
      test,
      R"({"ms_duration":{"count":1500,"unit":"milliseconds"},"custom_duration":{"count":10,"unit":"seconds"}})");
}

}  // namespace test_chrono_duration
