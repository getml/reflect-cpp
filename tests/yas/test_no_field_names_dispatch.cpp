#include <rfl/yas.hpp>

#include "../check_no_field_names_dispatch.hpp"
namespace test_no_field_names_dispatch {
TEST(yas, no_field_names_dispatch_roundtrip) {
  check_roundtrip(
      [](const auto& value) {
        return rfl::yas::write<rfl::NoFieldNames>(value);
      },
      []<class T>(const auto& bytes) {
        return rfl::yas::read<T, rfl::NoFieldNames>(bytes);
      });
}
}  // namespace test_no_field_names_dispatch
