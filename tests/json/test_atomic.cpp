#include <atomic>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_atomic {

struct Stats {
  std::atomic<std::uint64_t> bytes_downloaded;
  std::atomic<bool> finished;

  Stats(Stats&& other) noexcept
      : bytes_downloaded(
            other.bytes_downloaded.load(std::memory_order_relaxed)),
        finished(other.finished.load(std::memory_order_relaxed)) {}

  Stats& operator=(Stats&& other) noexcept {
    bytes_downloaded.store(
        other.bytes_downloaded.load(std::memory_order_relaxed),
        std::memory_order_relaxed);
    finished.store(other.finished.load(std::memory_order_relaxed),
                   std::memory_order_relaxed);
    return *this;
  }

  Stats(const std::uint64_t _bytes_downloaded, const bool _finished)
      : bytes_downloaded(_bytes_downloaded), finished(_finished) {}
};

TEST(json, test_atomic) {
  auto stats = Stats(123456789, true);

  write_and_read(stats, R"({"bytes_downloaded":123456789,"finished":true})");
}
}  // namespace test_atomic
