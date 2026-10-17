#include <gtest/gtest.h>

#include <optional>
#include <type_traits>

// Keep standard headers before the generated module import.
// clang-format off
#include <test_rfl/rfl.hpp>
// clang-format on

namespace test_box_default_constructor {

struct Recursive {
  std::optional<rfl::Box<Recursive>> next;
  std::optional<rfl::Ref<Recursive>> shared;
};

struct NoDefault {
  explicit NoDefault(int _value) : value(_value) {}
  int value;
};

static_assert(std::is_default_constructible_v<rfl::Box<Recursive>>);
static_assert(!std::is_default_constructible_v<rfl::Box<NoDefault>>);
static_assert(std::is_default_constructible_v<rfl::Ref<Recursive>>);
static_assert(!std::is_default_constructible_v<rfl::Ref<NoDefault>>);

TEST(generic, box_default_constructor) {
  const rfl::Box<Recursive> recursive;
  EXPECT_FALSE(recursive->next.has_value());
  const rfl::Box<int> number;
  EXPECT_EQ(*number, 0);
  const auto value = rfl::make_box<NoDefault>(42);
  EXPECT_EQ(value->value, 42);
  const rfl::Ref<Recursive> shared;
  EXPECT_FALSE(shared->shared.has_value());
}

}  // namespace test_box_default_constructor
