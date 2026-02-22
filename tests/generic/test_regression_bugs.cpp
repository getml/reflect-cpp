#include <gtest/gtest.h>

#include <climits>
#include <compare>
#include <concepts>
#include <cstdint>
#include <string>
#include <type_traits>

#include <rfl.hpp>

// Tuple::operator<=> always returns equivalent (inverted condition)
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

// Result::operator=(const Result<U>&) does not update success_
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

// Validator::operator=(U&&) is noexcept but calls .value() which
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

// Generic::to_int() silently truncates int64_t → int
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

// Validator::get() returns mutable reference, bypassing validation
// File: include/rfl/Validator.hpp:98-99
// Non-const get() returns T&, allowing direct write that skips validation.
namespace test_validator_mutable_get {

using PositiveInt = rfl::Validator<int, rfl::Minimum<0>>;

TEST(regression, validator_get_does_not_allow_invalid_mutation) {
  // After fix: get() returns const T&, so assignment through it is impossible
  constexpr bool can_assign_through_get =
      std::is_assignable_v<decltype(std::declval<PositiveInt&>().get()), int>;
  EXPECT_FALSE(can_assign_through_get)
      << "Validator::get() should return const T&, preventing direct mutation";
}

}  // namespace test_validator_mutable_get

// Box/Ref operator<=> compares pointer addresses, not values
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

// Field cross-type move constructor copies instead of moving
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

// Result::value_or(T&&) returns T&& instead of T
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

// TaggedUnion has typo: `discrimininator_` instead of `discriminator_`
// File: include/rfl/TaggedUnion.hpp:15
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

// Result::error() && returns Error& instead of Error&&
// File: include/rfl/Result.hpp:338
// The rvalue-qualified overload returns Error& but get_err() && returns Error&&.
// Should return Error&& for consistency and move semantics.
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

// Object::difference_type is unsigned (size_type) instead of signed
// File: include/rfl/Object.hpp:29
// C++ standard requires difference_type to be a signed integer type.
// Using size_type (unsigned) violates container requirements.
namespace test_object_difference_type_signed {

TEST(regression, object_difference_type_is_signed) {
  using DiffType = rfl::Object<int>::difference_type;
  EXPECT_TRUE(std::is_signed_v<DiffType>)
      << "Object::difference_type should be signed per C++ standard, "
         "but is unsigned (size_type)";
}

}  // namespace test_object_difference_type_signed

// Literal(const std::string&) is implicit — can throw unexpectedly
// File: include/rfl/Literal.hpp:46
// Constructor is not explicit, allowing implicit conversion from std::string
// which throws std::runtime_error on invalid input.
namespace test_literal_implicit_constructor {

TEST(regression, literal_constructor_is_explicit) {
  using LitType = rfl::Literal<"a", "b", "c">;
  constexpr bool is_implicit =
      std::is_convertible_v<std::string, LitType>;
  EXPECT_FALSE(is_implicit)
      << "Literal(const std::string&) should be explicit to prevent "
         "unexpected exceptions from implicit conversions";
}

}  // namespace test_literal_implicit_constructor

// Flatten cross-type move constructor copies instead of moving
// File: include/rfl/Flatten.hpp:32
// `value_(_f.get())` calls get() which returns lvalue ref → copy ctor.
// Should be `value_(std::move(_f.get()))`.
namespace test_flatten_cross_move {

struct FlatDerived;

struct FlatBase {
  int copies = 0;
  int moves = 0;
  bool from_rvalue = false;

  FlatBase() = default;
  FlatBase(const FlatBase& other)
      : copies(other.copies + 1), moves(other.moves) {}
  FlatBase(FlatBase&& other) noexcept
      : copies(other.copies), moves(other.moves + 1) {}
  FlatBase(const FlatDerived& other);
  FlatBase(FlatDerived&& other) noexcept;
  FlatBase& operator=(const FlatBase&) = default;
  FlatBase& operator=(FlatBase&&) noexcept = default;
};

struct FlatDerived {
  int copies = 0;
  int moves = 0;

  FlatDerived() = default;
  FlatDerived(const FlatDerived& other)
      : copies(other.copies + 1), moves(other.moves) {}
  FlatDerived(FlatDerived&& other) noexcept
      : copies(other.copies), moves(other.moves + 1) {}
};

inline FlatBase::FlatBase(const FlatDerived& other)
    : copies(other.copies), moves(other.moves), from_rvalue(false) {}
inline FlatBase::FlatBase(FlatDerived&& other) noexcept
    : copies(other.copies), moves(other.moves), from_rvalue(true) {}

TEST(regression, flatten_cross_type_move_does_not_copy) {
  auto source = rfl::Flatten<FlatDerived>(FlatDerived{});
  source.get().copies = 0;
  source.get().moves = 0;

  // Flatten(Flatten<U>&& _f) uses `value_(_f.get())` — get() returns
  // lvalue ref, so FlatBase(const FlatDerived&) is called instead of
  // FlatBase(FlatDerived&&). The fix would be `value_(std::move(_f.get()))`.
  auto dest = rfl::Flatten<FlatBase>(std::move(source));
  EXPECT_TRUE(dest.get().from_rvalue)
      << "Flatten cross-type move constructor should use rvalue conversion, "
         "but get() returns lvalue ref, so lvalue conversion is used instead";
}

}  // namespace test_flatten_cross_move

// Flatten(U&&) universal reference constructor copies instead of forwarding
// File: include/rfl/Flatten.hpp:40
// `value_(_value)` — _value is a named parameter (lvalue), so copy ctor is
// called even when an rvalue is passed. Should be `value_(std::forward<U>(_value))`.
// Note: When U == Type, the exact-match Flatten(Type&&) on line 22 is selected,
// so the bug only manifests for cross-type construction via Flatten(U&&).
namespace test_flatten_universal_ref_ctor {

using test_flatten_cross_move::FlatBase;
using test_flatten_cross_move::FlatDerived;

TEST(regression, flatten_universal_ref_ctor_forwards_rvalue) {
  auto val = FlatDerived{};
  val.copies = 0;
  val.moves = 0;

  // Cross-type rvalue: Flatten<FlatBase>(FlatDerived&&) selects Flatten(U&&)
  // where U = FlatDerived. Without std::forward, _value is an lvalue →
  // FlatBase(const FlatDerived&) is called instead of FlatBase(FlatDerived&&).
  auto flat = rfl::Flatten<FlatBase>(std::move(val));
  EXPECT_TRUE(flat.get().from_rvalue)
      << "Flatten(U&&) should forward rvalue to Type's converting "
         "move ctor, but value_(_value) without std::forward causes a copy";
}

}  // namespace test_flatten_universal_ref_ctor

// Flatten::operator=(Flatten<U>&&) uses std::forward<U>(_f) which passes
// Flatten<U> instead of U — fails to compile for cross-type assignment.
// File: include/rfl/Flatten.hpp:103-105
// Should be `value_ = std::move(_f.get())`.
// Tested via compile test: generic/compile_fail/flatten_cross_type_move_assign.cpp

// Skip cross-type move constructor copies instead of moving
// File: include/rfl/internal/Skip.hpp:43
// Same issue as Flatten: `value_(_other.get())` copies instead of moving.
namespace test_skip_cross_move {

struct SkipDerived2;

struct SkipBase {
  int copies = 0;
  int moves = 0;
  bool from_rvalue = false;

  SkipBase() = default;
  SkipBase(const SkipBase& other)
      : copies(other.copies + 1), moves(other.moves) {}
  SkipBase(SkipBase&& other) noexcept
      : copies(other.copies), moves(other.moves + 1) {}
  SkipBase(const SkipDerived2& other);
  SkipBase(SkipDerived2&& other) noexcept;
  SkipBase& operator=(const SkipBase&) = default;
  SkipBase& operator=(SkipBase&&) noexcept = default;
};

struct SkipDerived2 {
  int copies = 0;
  int moves = 0;

  SkipDerived2() = default;
  SkipDerived2(const SkipDerived2& other)
      : copies(other.copies + 1), moves(other.moves) {}
  SkipDerived2(SkipDerived2&& other) noexcept
      : copies(other.copies), moves(other.moves + 1) {}
};

inline SkipBase::SkipBase(const SkipDerived2& other)
    : copies(other.copies), moves(other.moves), from_rvalue(false) {}
inline SkipBase::SkipBase(SkipDerived2&& other) noexcept
    : copies(other.copies), moves(other.moves), from_rvalue(true) {}

TEST(regression, skip_cross_type_move_does_not_copy) {
  auto source =
      rfl::internal::Skip<SkipDerived2, true, true>(SkipDerived2{});
  source.get().copies = 0;
  source.get().moves = 0;

  auto dest = rfl::internal::Skip<SkipBase, true, true>(std::move(source));
  EXPECT_TRUE(dest.get().from_rvalue)
      << "Skip cross-type move constructor should use rvalue conversion, "
         "but get() returns lvalue ref, so lvalue conversion is used instead";
}

}  // namespace test_skip_cross_move

// Timestamp::reflection() does not check strftime return value
// File: include/rfl/Timestamp.hpp:100-102
// NOT TESTABLE: The format is a compile-time template parameter, so we cannot
// create a format that exceeds the 200-char output buffer at runtime.
// Any format long enough to overflow would also need to be parseable by
// strptime from a matching input string.
