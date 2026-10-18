#include <rfl.hpp>
#include <rfl/yas.hpp>

#include "../check_fail_fast.hpp"

TEST(yas, test_fail_fast_round_trip) {
  test_fail_fast::check_round_trip(
      [](const auto& input) { return rfl::yas::write(input); },
      []<class T>(const auto& bytes) {
        return rfl::yas::read<T, rfl::FailFast>(bytes);
      });
}

TEST(yas, test_fail_fast_truncated_input) {
  test_fail_fast::check_truncated(
      [](const auto& input) { return rfl::yas::write(input); },
      []<class T>(const auto& bytes) {
        return rfl::yas::read<T, rfl::FailFast>(bytes);
      });
}

TEST(yas, test_fail_fast_error_policy) {
  test_fail_fast::check_error_policy(
      [](const auto& input) { return rfl::yas::write(input); },
      []<class T>(const auto& bytes) { return rfl::yas::read<T>(bytes); },
      []<class T>(const auto& bytes) {
        return rfl::yas::read<T, rfl::FailFast>(bytes);
      });
}
