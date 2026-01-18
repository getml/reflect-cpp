#include <atomic>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_atomic_ref {

struct Stats {
  std::atomic<std::uint64_t> bytes_downloaded;
  std::atomic<bool> finished;
};

TEST(json, test_atomic_ref) {
  auto stats = rfl::make_ref<Stats>(123456789, true);

  write_and_read(stats, R"({"bytes_downloaded":123456789,"finished":true})");
}
}  // namespace test_atomic_ref
