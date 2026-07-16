#include <atomic>
#include <string>

#include "write_and_read.hpp"
#include <cstdint>

import rfl;
import rfl.json;


namespace test_atomic_box {

struct Stats {
  std::atomic<std::uint64_t> bytes_downloaded;
  std::atomic<bool> finished;
};

TEST(json, test_atomic_box) {
  auto stats = rfl::make_box<Stats>(123456789, true);

  write_and_read(stats, R"({"bytes_downloaded":123456789,"finished":true})");
}
}  // namespace test_atomic_box
