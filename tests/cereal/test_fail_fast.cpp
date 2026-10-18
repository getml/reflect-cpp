#include <rfl.hpp>
#include <rfl/cereal.hpp>

#include "../check_fail_fast.hpp"

TEST(cereal, test_fail_fast_round_trip) {
  test_fail_fast::check_round_trip(
      [](const auto& input) { return rfl::cereal::write(input); },
      []<class T>(const auto& bytes) {
        return rfl::cereal::read<T, rfl::FailFast>(bytes);
      });
}

TEST(cereal, test_fail_fast_truncated_input) {
  test_fail_fast::check_truncated(
      [](const auto& input) { return rfl::cereal::write(input); },
      []<class T>(const auto& bytes) {
        return rfl::cereal::read<T, rfl::FailFast>(bytes);
      });
}

TEST(cereal, test_fail_fast_error_policy) {
  test_fail_fast::check_error_policy(
      [](const auto& input) { return rfl::cereal::write(input); },
      []<class T>(const auto& bytes) { return rfl::cereal::read<T>(bytes); },
      []<class T>(const auto& bytes) {
        return rfl::cereal::read<T, rfl::FailFast>(bytes);
      });
}
