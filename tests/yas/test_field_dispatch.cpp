#include <rfl/yas.hpp>

#include "../check_positional_field_dispatch.hpp"
namespace test_positional_field_dispatch {
const auto write_yas = [](const auto& input) { return rfl::yas::write(input); };
const auto read_yas = []<class T>(const auto& bytes) {
  return rfl::yas::read<T>(bytes);
};
TEST(yas, field_dispatch_errors_and_cleanup) {
  check_errors(write_yas, read_yas);
}
TEST(yas, field_dispatch_empty_object) { check_empty(write_yas, read_yas); }
TEST(yas, field_dispatch_truncated_cleanup) {
  check_truncated(write_yas, read_yas);
}
TEST(yas, field_dispatch_static_indices) {
  int next = 0;
  int static_calls = 0;
  const rfl::yas::Reader reader;
  EXPECT_FALSE(reader.read_object(
      IndexedReader<rfl::yas::Reader, 3>{{&next}, &static_calls}, {nullptr}));
  EXPECT_EQ(next, 3);
  EXPECT_EQ(static_calls, 3);
}
}  // namespace test_positional_field_dispatch
