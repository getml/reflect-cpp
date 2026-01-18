#include <atomic>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_atomic_array {

struct Stats {
  std::atomic<std::uint64_t> bytes_downloaded;
  std::atomic<bool> finished;
};

TEST(json, test_atomic_array) {
  auto arr = rfl::Ref<std::array<Stats, 2>>{};
  (*arr)[0].bytes_downloaded = 123456789;
  (*arr)[0].finished = true;
  (*arr)[1].bytes_downloaded = 987654321;
  (*arr)[1].finished = false;

  write_and_read(
      arr,
      R"([{"bytes_downloaded":123456789,"finished":true},{"bytes_downloaded":987654321,"finished":false}])");
}
}  // namespace test_atomic_array
