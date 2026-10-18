#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <vector>

#include <test_rfl/rfl.cbor.hpp>

namespace test_cbor_stream_bridge {
TEST(cbor, scalar_wire_and_stream) {
  const std::vector<unsigned char> expected{0x18, 0x2a};
  const auto bytes = rfl::cbor::write(42);
  EXPECT_EQ(std::vector<unsigned char>(bytes.begin(), bytes.end()), expected);
  const auto result = rfl::cbor::read<int>(expected);
  ASSERT_TRUE(result);
  EXPECT_EQ(result.value(), 42);

  std::stringstream stream;
  rfl::cbor::write(42, stream);
  EXPECT_EQ(stream.str(), std::string(bytes.begin(), bytes.end()));
  const auto streamed = rfl::cbor::read<int>(stream);
  ASSERT_TRUE(streamed);
  EXPECT_EQ(streamed.value(), 42);
}

TEST(cbor, malformed_stream_reports_decoder_error) {
  std::stringstream stream(std::string(1, static_cast<char>(0xff)));
  const auto result = rfl::cbor::read<int>(stream);
  ASSERT_FALSE(result);
  EXPECT_TRUE(result.error().what().starts_with("Could not parse CBOR: "));
}
}  // namespace test_cbor_stream_bridge
