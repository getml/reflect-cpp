#include <rfl/msgpack.hpp>

#include "../check_no_field_names_dispatch.hpp"
namespace test_no_field_names_dispatch {
TEST(msgpack, no_field_names_dispatch_roundtrip) {
  check_roundtrip(
      [](const auto& value) {
        return rfl::msgpack::write<rfl::NoFieldNames>(value);
      },
      []<class T>(const auto& bytes) {
        return rfl::msgpack::read<T, rfl::NoFieldNames>(bytes);
      });
}
}  // namespace test_no_field_names_dispatch
