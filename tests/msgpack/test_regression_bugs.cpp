#include <gtest/gtest.h>

#include <msgpack.h>

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include <rfl.hpp>
#include <rfl/msgpack.hpp>

// msgpack::read — ignores return value of msgpack_unpack
// File: include/rfl/msgpack/read.hpp:35-36
// msgpack_unpack() return value is not checked. If unpacking fails,
// the deserialized object contains uninitialized data, but the code
// continues to parse it. The error is caught downstream by type mismatch,
// producing a misleading error message ("Could not cast to a map")
// instead of "invalid msgpack data".
namespace test_msgpack_read_corrupt_data {

struct Simple {
  std::string name;
  int value;
};

TEST(msgpack_regression, read_single_invalid_byte_returns_meaningful_error) {
  // A single 0xC1 byte is reserved/unused in msgpack spec.
  // msgpack_unpack should fail, and the error should be caught early.
  const char buf[] = {'\xC1'};
  const auto res = rfl::msgpack::read<Simple>(buf, sizeof(buf));
  ASSERT_FALSE(res) << "Reading invalid msgpack byte 0xC1 should fail";
  const std::string msg = res.error().what();
  // With the bug: error is a downstream type mismatch like "Could not cast
  // to a map" because unpack failure is ignored and garbage is parsed.
  // After fix: error should mention msgpack unpacking/parsing failure.
  EXPECT_EQ(msg.find("cast"), std::string::npos)
      << "Error for invalid msgpack should come from unpack check, "
         "not from a downstream type cast. Got: " << msg;
}

TEST(msgpack_regression, read_truncated_data_returns_meaningful_error) {
  const auto obj = Simple{.name = "hello world", .value = 42};
  const auto valid_bytes = rfl::msgpack::write(obj);
  ASSERT_GT(valid_bytes.size(), 4u);

  // Truncate to 2 bytes
  const auto res = rfl::msgpack::read<Simple>(valid_bytes.data(), 2);
  ASSERT_FALSE(res) << "Reading truncated msgpack data should fail";
  const std::string msg = res.error().what();
  EXPECT_EQ(msg.find("cast"), std::string::npos)
      << "Error for truncated msgpack should come from unpack check, "
         "not from a downstream type cast. Got: " << msg;
}

}  // namespace test_msgpack_read_corrupt_data

// msgpack::Writer — uint64_t values above INT64_MAX written as signed int64
// File: include/rfl/msgpack/Writer.hpp:133-134
// All integer types are packed via msgpack_pack_int64(static_cast<int64_t>()).
// For uint64_t values > INT64_MAX, the cast wraps to negative, and msgpack
// stores them as NEGATIVE_INTEGER type instead of POSITIVE_INTEGER.
// This is incorrect for interop — other msgpack implementations will read
// a negative number instead of the intended large unsigned value.
namespace test_msgpack_writer_uint64 {

struct WithUint64 {
  uint64_t big_value;
};

TEST(msgpack_regression, uint64_above_int64_max_stored_as_positive) {
  const uint64_t large_value =
      static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) + 1u;
  const auto obj = WithUint64{.big_value = large_value};
  const auto bytes = rfl::msgpack::write(obj);

  // Parse the raw msgpack to check the wire type
  msgpack_zone mempool;
  msgpack_zone_init(&mempool, 2048);
  msgpack_object deserialized;
  const auto rc = msgpack_unpack(bytes.data(), bytes.size(), nullptr,
                                 &mempool, &deserialized);
  ASSERT_EQ(rc, MSGPACK_UNPACK_SUCCESS);

  ASSERT_EQ(deserialized.type, MSGPACK_OBJECT_MAP);
  ASSERT_EQ(deserialized.via.map.size, 1u);
  const auto& val = deserialized.via.map.ptr[0].val;

  // With the bug: value is NEGATIVE_INTEGER (because int64_t cast wraps)
  // After fix: value should be POSITIVE_INTEGER
  EXPECT_EQ(val.type, MSGPACK_OBJECT_POSITIVE_INTEGER)
      << "uint64_t value " << large_value
      << " should be stored as POSITIVE_INTEGER in msgpack, "
         "but was stored as type " << static_cast<int>(val.type)
      << " (NEGATIVE_INTEGER=" << MSGPACK_OBJECT_NEGATIVE_INTEGER << ")";

  msgpack_zone_destroy(&mempool);
}

TEST(msgpack_regression, uint64_max_stored_as_positive) {
  const auto obj =
      WithUint64{.big_value = std::numeric_limits<uint64_t>::max()};
  const auto bytes = rfl::msgpack::write(obj);

  msgpack_zone mempool;
  msgpack_zone_init(&mempool, 2048);
  msgpack_object deserialized;
  const auto rc = msgpack_unpack(bytes.data(), bytes.size(), nullptr,
                                 &mempool, &deserialized);
  ASSERT_EQ(rc, MSGPACK_UNPACK_SUCCESS);
  ASSERT_EQ(deserialized.type, MSGPACK_OBJECT_MAP);
  ASSERT_EQ(deserialized.via.map.size, 1u);
  const auto& val = deserialized.via.map.ptr[0].val;

  EXPECT_EQ(val.type, MSGPACK_OBJECT_POSITIVE_INTEGER)
      << "UINT64_MAX should be stored as POSITIVE_INTEGER, "
         "not NEGATIVE_INTEGER";

  msgpack_zone_destroy(&mempool);
}

}  // namespace test_msgpack_writer_uint64
