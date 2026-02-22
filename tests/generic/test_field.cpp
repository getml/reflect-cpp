#include <gtest/gtest.h>

#include <rfl.hpp>

#include "test_helpers.hpp"

namespace test_field_cross_move {

using test_helpers::DerivedTracker;
using test_helpers::MoveTracker;

TEST(regression, field_cross_type_move_does_not_copy) {
  auto source = rfl::Field<"x", DerivedTracker>(DerivedTracker{});
  source.get().copies = 0;
  source.get().moves = 0;

  auto dest = rfl::Field<"x", MoveTracker>(std::move(source));
  EXPECT_EQ(dest.get().copies, 0)
      << "Field cross-type move constructor should move, not copy. "
         "get() returns lvalue ref, causing copy instead of move";
}

}  // namespace test_field_cross_move
