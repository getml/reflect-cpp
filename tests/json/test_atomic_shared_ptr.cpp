#include <atomic>
#include <string>

#include <cstdint>
#include <memory>

#include "write_and_read.hpp"

#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


namespace test_atomic_shared_ptr {

struct Stats {
  std::atomic<std::uint64_t> bytes_downloaded;
  std::atomic<bool> finished;
};

TEST(json, test_atomic_shared_ptr) {
  auto stats = std::make_shared<Stats>(123456789, true);

  write_and_read(stats, R"({"bytes_downloaded":123456789,"finished":true})");
}
}  // namespace test_atomic_shared_ptr
