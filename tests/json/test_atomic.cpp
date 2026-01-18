#include <atomic>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_atomic {

struct Stats {
  std::atomic<std::uint64_t> bytes_downloaded;
  std::atomic<bool> finished;
  rfl::Ref<std::atomic<int>> ref_atomic_int;
  rfl::Box<std::atomic<int>> box_atomic_int;
  std::shared_ptr<std::atomic<int>> shared_atomic_int;
  std::unique_ptr<std::atomic<int>> unique_atomic_int;
};

TEST(json, test_atomic) {
  auto stats =
      Stats{.bytes_downloaded = 123456789,
            .finished = true,
            .ref_atomic_int = rfl::make_ref<std::atomic<int>>(42),
            .box_atomic_int = rfl::Box<std::atomic<int>>::make(7),
            .shared_atomic_int = std::make_shared<std::atomic<int>>(13),
            .unique_atomic_int = std::make_unique<std::atomic<int>>(21)};

  const auto json_str = rfl::json::write(stats);

  Stats stats2{};

  const auto res =
      rfl::json::read<rfl::atomic::remove_atomic_t<Stats>>(json_str).transform(
          [&](auto&& t) {
            return rfl::atomic::set_atomic(std::move(t), &stats2);
          });

  ASSERT_TRUE(res.has_value()) << res.error().what();
  EXPECT_EQ(rfl::json::write(stats2), json_str);
  EXPECT_EQ(
      json_str,
      R"({"bytes_downloaded":123456789,"finished":true,"ref_atomic_int":42,"box_atomic_int":7,"shared_atomic_int":13,"unique_atomic_int":21})");
}
}  // namespace test_atomic
