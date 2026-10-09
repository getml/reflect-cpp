#include <chrono>
#include <rfl.hpp>
#include <rfl/json.hpp>

#include "write_and_read.hpp"

namespace test_durations_as_count {

struct TestStruct {
  std::chrono::milliseconds ms;
  std::chrono::seconds s;
  std::chrono::duration<double> d;
};

TEST(json, test_durations_as_count) {
  const auto test = TestStruct{.ms = std::chrono::milliseconds(1500),
                               .s = std::chrono::seconds(10),
                               .d = std::chrono::duration<double>(0.5)};
  write_and_read<rfl::DurationsAsCount>(test,
                                        R"({"ms":1500,"s":10,"d":0.5})");
}
}  // namespace test_durations_as_count
