#include <rfl/cereal.hpp>

#include "../check_no_field_names_dispatch.hpp"
namespace test_no_field_names_dispatch {
TEST(cereal, no_field_names_dispatch_roundtrip) {
  check_roundtrip(
      [](const auto& value) {
        return rfl::cereal::write<rfl::NoFieldNames>(value);
      },
      []<class T>(const auto& bytes) {
        return rfl::cereal::read<T, rfl::NoFieldNames>(bytes);
      });
}
}  // namespace test_no_field_names_dispatch
