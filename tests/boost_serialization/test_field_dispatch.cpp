#include <rfl/boost_serialization.hpp>

#include "../check_positional_field_dispatch.hpp"
namespace test_positional_field_dispatch {
const auto write_boost_serialization = [](const auto& input) {
  return rfl::boost_serialization::write(input);
};
const auto read_boost_serialization = []<class T>(const auto& bytes) {
  return rfl::boost_serialization::read<T>(bytes);
};
TEST(boost_serialization, field_dispatch_errors_and_cleanup) {
  check_errors(write_boost_serialization, read_boost_serialization);
}
TEST(boost_serialization, field_dispatch_empty_object) {
  check_empty(write_boost_serialization, read_boost_serialization);
}
TEST(boost_serialization, field_dispatch_truncated_cleanup) {
  check_truncated(write_boost_serialization, read_boost_serialization);
}
TEST(boost_serialization, field_dispatch_input_counts) {
  const rfl::boost_serialization::Reader<boost::archive::binary_iarchive>
      reader;
  for (const auto count : {0, 2, 3, 4}) {
    check_counts(reader, {nullptr, static_cast<std::size_t>(count)}, count);
  }
}
}  // namespace test_positional_field_dispatch
