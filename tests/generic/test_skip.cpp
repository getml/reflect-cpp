#include <gtest/gtest.h>

#include <rfl.hpp>

#include "test_helpers.hpp"

namespace test_skip_cross_move {

using test_helpers::SkipBase;
using test_helpers::SkipDerived;

TEST(regression, skip_cross_type_move_does_not_copy) {
  auto source =
      rfl::internal::Skip<SkipDerived, true, true>(SkipDerived{});
  source.get().copies = 0;
  source.get().moves = 0;

  auto dest = rfl::internal::Skip<SkipBase, true, true>(std::move(source));
  EXPECT_TRUE(dest.get().from_rvalue)
      << "Skip cross-type move constructor should use rvalue conversion, "
         "but get() returns lvalue ref, so lvalue conversion is used instead";
}

}  // namespace test_skip_cross_move
