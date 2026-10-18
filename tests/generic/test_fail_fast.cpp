#include <rfl.hpp>
#include <rfl/from_generic.hpp>
#include <rfl/to_generic.hpp>

#include "../check_fail_fast.hpp"

TEST(generic, test_fail_fast_round_trip) {
  test_fail_fast::check_round_trip(
      [](const auto& input) { return rfl::to_generic(input); },
      []<class T>(const auto& bytes) {
        return rfl::from_generic<T, rfl::FailFast>(bytes);
      });
}

TEST(generic, test_fail_fast_error_policy) {
  test_fail_fast::check_error_policy(
      [](const auto& input) { return rfl::to_generic(input); },
      []<class T>(const auto& bytes) { return rfl::from_generic<T>(bytes); },
      []<class T>(const auto& bytes) {
        return rfl::from_generic<T, rfl::FailFast>(bytes);
      });
}
