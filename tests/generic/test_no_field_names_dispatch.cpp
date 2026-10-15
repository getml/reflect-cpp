#include <rfl.hpp>

#include "../check_no_field_names_dispatch.hpp"

namespace test_no_field_names_dispatch {
TEST(generic, no_field_names_dispatch_roundtrip) {
  const Data input{{{1, "first", 2}, {-3, "second", 4}}};
  const auto value = rfl::to_generic<rfl::NoFieldNames>(input);
  const auto result = rfl::from_generic<Data, rfl::NoFieldNames>(value);
  ASSERT_TRUE(result);
  EXPECT_EQ(*result, input);
}
}  // namespace test_no_field_names_dispatch
