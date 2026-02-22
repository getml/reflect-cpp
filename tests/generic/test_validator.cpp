#include <gtest/gtest.h>

#include <type_traits>

#include <rfl.hpp>

namespace test_validator_noexcept {

using PositiveInt = rfl::Validator<int, rfl::Minimum<0>>;

TEST(regression, validator_assign_rvalue_operator_is_noexcept_but_can_throw) {
  constexpr bool is_nothrow =
      std::is_nothrow_assignable_v<PositiveInt&, long&&>;
  EXPECT_FALSE(is_nothrow)
      << "Validator::operator=(U&&) is marked noexcept but calls .value() "
         "which throws on validation failure — this causes std::terminate";
}

}  // namespace test_validator_noexcept

namespace test_validator_mutable_get {

using PositiveInt = rfl::Validator<int, rfl::Minimum<0>>;

TEST(regression, validator_get_does_not_allow_invalid_mutation) {
  constexpr bool can_assign_through_get =
      std::is_assignable_v<decltype(std::declval<PositiveInt&>().get()), int>;
  EXPECT_FALSE(can_assign_through_get)
      << "Validator::get() should return const T&, preventing direct mutation";
}

}  // namespace test_validator_mutable_get
