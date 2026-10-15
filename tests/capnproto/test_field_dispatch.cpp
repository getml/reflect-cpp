#include <rfl/capnproto.hpp>

#include "../check_positional_field_dispatch.hpp"
namespace test_positional_field_dispatch {
const auto write_capnproto = [](const auto& input) {
  return rfl::capnproto::write(input);
};
const auto read_capnproto = []<class T>(const auto& bytes) {
  return rfl::capnproto::read<T>(bytes);
};
TEST(capnproto, field_dispatch_errors_and_cleanup) {
  check_errors(write_capnproto, read_capnproto);
}
TEST(capnproto, field_dispatch_empty_object) {
  check_empty(write_capnproto, read_capnproto);
}
TEST(capnproto, field_dispatch_schema_counts) {
  const auto schema = rfl::capnproto::to_schema<Input>();
  const auto name = rfl::capnproto::get_root_name<Input>();
  const auto root_schema = schema.value().getNested(name.c_str()).asStruct();
  capnp::MallocMessageBuilder message;
  const auto root = message.initRoot<capnp::DynamicStruct>(root_schema);
  const rfl::capnproto::Reader reader;
  check_counts(reader, {root.asReader()}, 3);
}
}  // namespace test_positional_field_dispatch
