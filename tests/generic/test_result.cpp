#include <gtest/gtest.h>

#include <string>
#include <type_traits>

#include <rfl.hpp>

namespace test_result_cross_assign {

TEST(regression, result_cross_type_assign_error_clears_success) {
  auto ok = rfl::Result<int>(42);
  ASSERT_TRUE(ok);

  const auto err = rfl::Result<long>(rfl::Error("fail"));
  ASSERT_FALSE(err);

  ok = err;
  EXPECT_FALSE(ok)
      << "After assigning an error Result<long> to Result<int>, "
         "the target should be falsy";
}

}  // namespace test_result_cross_assign

namespace test_result_value_or_return_type {

TEST(regression, result_value_or_returns_value_not_rvalue_ref) {
  using ResultType = rfl::Result<std::string>;
  using ReturnType = decltype(
      std::declval<ResultType&&>().value_or(std::string("default")));
  constexpr bool returns_value = std::is_same_v<ReturnType, std::string>;
  EXPECT_TRUE(returns_value)
      << "Result::value_or(T&&) should return T by value, not T&&. "
         "Returning T&& can create dangling references.";
}

}  // namespace test_result_value_or_return_type

namespace test_result_error_rvalue_return_type {

TEST(regression, result_error_rvalue_qualified_returns_rvalue_ref) {
  using ResultType = rfl::Result<int>;
  using ReturnType =
      decltype(std::declval<ResultType&&>().error());
  constexpr bool returns_rvalue_ref = std::is_same_v<ReturnType, rfl::Error&&>;
  constexpr bool returns_lvalue_ref = std::is_same_v<ReturnType, rfl::Error&>;
  EXPECT_TRUE(returns_rvalue_ref)
      << "Result::error() && should return Error&&, not Error&. "
         "Currently returns lvalue ref: " << returns_lvalue_ref;
}

}  // namespace test_result_error_rvalue_return_type
