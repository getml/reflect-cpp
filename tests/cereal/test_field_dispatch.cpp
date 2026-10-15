#include <rfl/cereal.hpp>

#include "../check_positional_field_dispatch.hpp"
namespace test_positional_field_dispatch {
const auto write_cereal = [](const auto& input) {
  return rfl::cereal::write(input);
};
const auto read_cereal = []<class T>(const auto& bytes) {
  return rfl::cereal::read<T>(bytes);
};
TEST(cereal, field_dispatch_errors_and_cleanup) {
  check_errors(write_cereal, read_cereal);
}
TEST(cereal, field_dispatch_empty_object) {
  check_empty(write_cereal, read_cereal);
}
TEST(cereal, field_dispatch_truncated_cleanup) {
  check_truncated(write_cereal, read_cereal);
}
TEST(cereal, field_dispatch_static_indices) {
  int next = 0;
  int static_calls = 0;
  const rfl::cereal::Reader reader;
  EXPECT_FALSE(reader.read_object(
      IndexedReader<rfl::cereal::Reader, 3>{{&next}, &static_calls},
      {nullptr}));
  EXPECT_EQ(next, 3);
  EXPECT_EQ(static_calls, 3);
}
struct CerealRuntimeIndexReader : RuntimeIndexReader<rfl::cereal::Reader> {
  static constexpr int size() { return 3; }
};
TEST(cereal, field_dispatch_runtime_index_reader_compatibility) {
  int next = 0;
  const rfl::cereal::Reader reader;
  EXPECT_FALSE(
      reader.read_object(CerealRuntimeIndexReader{{&next}}, {nullptr}));
  EXPECT_EQ(next, 3);
}
}  // namespace test_positional_field_dispatch
