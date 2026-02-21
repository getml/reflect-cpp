#include <gtest/gtest.h>

#include <climits>
#include <compare>
#include <concepts>
#include <cstdint>
#include <string>
#include <type_traits>

#include <rfl.hpp>

// Bug 1.1 — Tuple::operator<=> always returns equivalent (inverted condition)
// File: include/rfl/Tuple.hpp:107
// The condition checks `_ordering != equivalent && elements differ` but should
// check `_ordering == equivalent && elements differ` (or simply always compare
// when equivalent).
namespace test_tuple_spaceship {

TEST(regression, tuple_spaceship_not_always_equivalent) {
  const auto a = rfl::Tuple<int>(1);
  const auto b = rfl::Tuple<int>(2);
  EXPECT_NE(a <=> b, std::strong_ordering::equivalent)
      << "Tuple<int>(1) <=> Tuple<int>(2) should not be equivalent";
}

TEST(regression, tuple_spaceship_less) {
  const auto a = rfl::Tuple<int>(1);
  const auto b = rfl::Tuple<int>(2);
  EXPECT_EQ(a <=> b, std::strong_ordering::less)
      << "Tuple<int>(1) <=> Tuple<int>(2) should be less";
}

}  // namespace test_tuple_spaceship

// Bug 1.2 — Result::operator=(const Result<U>&) does not update success_
// File: include/rfl/Result.hpp:210-216
// The operator copies t_or_err_ from _other.transform(...) but never updates
// success_ flag.
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

// Bug 1.3 — Validator::operator=(U&&) is noexcept but calls .value() which
// throws File: include/rfl/Validator.hpp:77
// If validation fails, .value() throws inside a noexcept function →
// std::terminate.
namespace test_validator_noexcept {

using PositiveInt = rfl::Validator<int, rfl::Minimum<0>>;

TEST(regression, validator_assign_rvalue_operator_is_noexcept_but_can_throw) {
  // The template operator=(U&&) at line 77 is marked noexcept, but internally
  // calls .value() which throws when validation fails. In a noexcept context
  // this causes std::terminate instead of a catchable exception.
  // The template overload is selected for cross-type assignment (U != T).
  constexpr bool is_nothrow =
      std::is_nothrow_assignable_v<PositiveInt&, long&&>;
  EXPECT_FALSE(is_nothrow)
      << "Validator::operator=(U&&) is marked noexcept but calls .value() "
         "which throws on validation failure — this causes std::terminate";
}

}  // namespace test_validator_noexcept

// Bug 1.14 — Generic::to_int() silently truncates int64_t → int
// File: include/rfl/Generic.hpp:175-188
// Does static_cast<int>(_v) without range check. Values outside [INT_MIN,
// INT_MAX] are silently truncated.
namespace test_generic_to_int_truncation {

TEST(regression, generic_to_int_rejects_overflow) {
  const int64_t value = static_cast<int64_t>(INT_MAX) + 1;
  const auto big = rfl::Generic(value);
  const auto result = big.to_int();
  EXPECT_FALSE(result)
      << "Generic::to_int() should fail for value " << value
      << " exceeding INT_MAX";
}

TEST(regression, generic_to_int_rejects_underflow) {
  const int64_t value = static_cast<int64_t>(INT_MIN) - 1;
  const auto small = rfl::Generic(value);
  const auto result = small.to_int();
  EXPECT_FALSE(result)
      << "Generic::to_int() should fail for value " << value
      << " below INT_MIN";
}

}  // namespace test_generic_to_int_truncation

// Bug 3.1 — Validator::get() returns mutable reference, bypassing validation
// File: include/rfl/Validator.hpp:98-99
// Non-const get() returns T&, allowing direct write that skips validation.
namespace test_validator_mutable_get {

using PositiveInt = rfl::Validator<int, rfl::Minimum<0>>;

TEST(regression, validator_get_does_not_allow_invalid_mutation) {
  auto v = PositiveInt(10);
  ASSERT_EQ(v.get(), 10);

  // Directly mutate through mutable get() — bypasses validation
  v.get() = -10;
  EXPECT_GE(v.get(), 0)
      << "Validator::get() returned a mutable reference that allowed "
         "writing -10, bypassing Minimum<0> validation";
}

}  // namespace test_validator_mutable_get

// Bug 3.2 — Box/Ref operator<=> compares pointer addresses, not values
// File: include/rfl/Box.hpp:130-132, include/rfl/Ref.hpp:118-119
namespace test_box_ref_spaceship {

TEST(regression, box_spaceship_compares_values_not_pointers) {
  const auto a = rfl::make_box<int>(42);
  const auto b = rfl::make_box<int>(42);
  EXPECT_EQ(a <=> b, std::strong_ordering::equivalent)
      << "Box<int>(42) <=> Box<int>(42) should compare values, not pointers";
}

TEST(regression, ref_spaceship_compares_values_not_pointers) {
  const auto a = rfl::make_ref<int>(42);
  const auto b = rfl::make_ref<int>(42);
  EXPECT_EQ(a <=> b, std::strong_ordering::equivalent)
      << "Ref<int>(42) <=> Ref<int>(42) should compare values, not pointers";
}

}  // namespace test_box_ref_spaceship

// Bug 3.9 — Field cross-type move constructor copies instead of moving
// File: include/rfl/Field.hpp:37
// `value_(_field.get())` calls get() which returns lvalue ref → copy ctor
namespace test_field_cross_move {

struct MoveTracker {
  int copies = 0;
  int moves = 0;

  MoveTracker() = default;
  MoveTracker(const MoveTracker& other)
      : copies(other.copies + 1), moves(other.moves) {}
  MoveTracker(MoveTracker&& other) noexcept
      : copies(other.copies), moves(other.moves + 1) {}
  MoveTracker& operator=(const MoveTracker&) = default;
  MoveTracker& operator=(MoveTracker&&) noexcept = default;
};

// Derived type that is convertible to base MoveTracker
struct DerivedTracker : MoveTracker {
  DerivedTracker() = default;
  DerivedTracker(const DerivedTracker& other) : MoveTracker(other) {}
  DerivedTracker(DerivedTracker&& other) noexcept
      : MoveTracker(std::move(other)) {}
};

TEST(regression, field_cross_type_move_does_not_copy) {
  auto source = rfl::Field<"x", DerivedTracker>(DerivedTracker{});
  // Reset counters after initial construction
  source.get().copies = 0;
  source.get().moves = 0;

  // Cross-type move: Field<"x", DerivedTracker> → Field<"x", MoveTracker>
  auto dest = rfl::Field<"x", MoveTracker>(std::move(source));
  EXPECT_EQ(dest.get().copies, 0)
      << "Field cross-type move constructor should move, not copy. "
         "get() returns lvalue ref, causing copy instead of move";
}

}  // namespace test_field_cross_move

// Bug 3.10 — Result::value_or(T&&) returns T&& instead of T
// File: include/rfl/Result.hpp:295
// Returning rvalue reference to a parameter is dangerous (dangling).
// Should return by value.
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

// Bug 3.13 — TaggedUnion has typo: `discrimininator_` instead of
// `discriminator_` File: include/rfl/TaggedUnion.hpp:15
namespace test_tagged_union_discriminator_name {

struct A {
  int x;
};

struct B {
  std::string y;
};

using TU = rfl::TaggedUnion<"type", A, B>;

template <class T>
concept has_discriminator = requires { T::discriminator_; };

template <class T>
concept has_discrimininator = requires { T::discrimininator_; };

TEST(regression, tagged_union_discriminator_spelling) {
  constexpr bool correct_name = has_discriminator<TU>;
  constexpr bool typo_name = has_discrimininator<TU>;
  EXPECT_TRUE(correct_name)
      << "TaggedUnion should have member 'discriminator_', "
         "not 'discrimininator_' (typo with extra 'n')";
  EXPECT_FALSE(typo_name)
      << "TaggedUnion has 'discrimininator_' (typo) which should be "
         "'discriminator_'";
}

}  // namespace test_tagged_union_discriminator_name
