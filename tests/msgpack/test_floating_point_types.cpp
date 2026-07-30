#include <gtest/gtest.h>

#include <msgpack.h>

#include <rfl/msgpack.hpp>

namespace test_floating_point_types {

TEST(msgpack, writes_float_and_double_with_matching_wire_types) {
  // Use equal numeric values so the C++ source type is the only difference.
  const auto float_bytes = rfl::msgpack::write(1.0F);
  const auto double_bytes = rfl::msgpack::write(1.0);

  // Inspect the raw type because normal round-tripping accepts both widths.
  msgpack_unpacked float_value;
  msgpack_unpacked_init(&float_value);
  ASSERT_TRUE(msgpack_unpack_next(&float_value, float_bytes.data(),
                                  float_bytes.size(), nullptr));
  EXPECT_EQ(float_value.data.type, MSGPACK_OBJECT_FLOAT32);
  msgpack_unpacked_destroy(&float_value);

  msgpack_unpacked double_value;
  msgpack_unpacked_init(&double_value);
  ASSERT_TRUE(msgpack_unpack_next(&double_value, double_bytes.data(),
                                  double_bytes.size(), nullptr));
  EXPECT_EQ(double_value.data.type, MSGPACK_OBJECT_FLOAT64);
  msgpack_unpacked_destroy(&double_value);
}

}  // namespace test_floating_point_types
