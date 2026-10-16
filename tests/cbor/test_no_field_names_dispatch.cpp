#include <rfl/cbor.hpp>

#include "../check_no_field_names_dispatch.hpp"
namespace test_no_field_names_dispatch {
TEST(cbor, no_field_names_dispatch_roundtrip) {
  check_roundtrip(
      [](const auto& value) {
        return rfl::cbor::write<rfl::NoFieldNames>(value);
      },
      []<class T>(const auto& bytes) {
        return rfl::cbor::read<T, rfl::NoFieldNames>(bytes);
      });
}
}  // namespace test_no_field_names_dispatch
