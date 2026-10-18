#include <rfl/avro.hpp>

#include "../check_positional_field_dispatch.hpp"
namespace test_positional_field_dispatch {
const auto write_avro = [](const auto& input) {
  return rfl::avro::write(input);
};
const auto read_avro = []<class T>(const auto& bytes) {
  return rfl::avro::read<T>(bytes);
};
TEST(avro, field_dispatch_errors_and_cleanup) {
  check_errors(write_avro, read_avro);
}
TEST(avro, field_dispatch_empty_object) { check_empty(write_avro, read_avro); }
TEST(avro, field_dispatch_schema_counts) {
  const auto schema = rfl::avro::to_schema<Input>();
  avro_value_t root;
  ASSERT_EQ(avro_generic_value_new(schema.iface(), &root), 0);
  const rfl::avro::Reader reader;
  check_counts(reader, {&root}, 3);
  avro_value_decref(&root);
}
}  // namespace test_positional_field_dispatch
