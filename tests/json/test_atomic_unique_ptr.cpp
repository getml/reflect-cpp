#include <atomic>
#include <string>

#include "write_and_read.hpp"
#include <cstdint>
#include <memory>

import rfl;
import rfl.json;


namespace test_atomic_unique_ptr {

struct Stats {
  std::atomic<std::uint64_t> bytes_downloaded;
  std::atomic<bool> finished;
};

TEST(json, test_atomic_unique_ptr) {
  auto stats = std::make_unique<Stats>(123456789, true);

  write_and_read(stats, R"({"bytes_downloaded":123456789,"finished":true})");
}
}  // namespace test_atomic_unique_ptr
