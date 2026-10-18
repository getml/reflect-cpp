#include <gtest/gtest.h>

#include <limits>
#include <sstream>
#include <test_rfl/rfl.capnproto.hpp>

#include "native_reference.hpp"

namespace test_capnproto_bridge {
TEST(capnproto, native_interoperability) {
  const Record expected{"module bridge 世界",
                        std::numeric_limits<int64_t>::min(),
                        true,
                        {0.5, -1.25, 0.0}};
  const auto schema = rfl::capnproto::to_schema<Record>();
  const auto bytes = rfl::capnproto::write(expected, schema);
  EXPECT_EQ(bytes, reference_bytes(schema.value(), expected));
  const auto native = reference_read(schema.value(), bytes);
  EXPECT_EQ(native.text, expected.text);
  EXPECT_EQ(native.count, expected.count);
  EXPECT_EQ(native.ready, expected.ready);
  EXPECT_EQ(native.values, expected.values);
  const auto parsed = rfl::capnproto::read<Record>(
      reference_bytes(schema.value(), expected), schema);
  ASSERT_TRUE(parsed);
  EXPECT_EQ(parsed->text, expected.text);
  EXPECT_EQ(parsed->count, expected.count);
  EXPECT_EQ(parsed->values, expected.values);

  std::stringstream stream;
  rfl::capnproto::write(expected, schema, stream);
  const auto streamed = rfl::capnproto::read<Record>(stream, schema);
  ASSERT_TRUE(streamed);
  EXPECT_EQ(streamed->values, expected.values);
}
}  // namespace test_capnproto_bridge
