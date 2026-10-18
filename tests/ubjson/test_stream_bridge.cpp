#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <vector>

#include <test_rfl/rfl.ubjson.hpp>

namespace test_ubjson_stream_bridge {
TEST(ubjson, scalar_wire_and_stream) {
  const std::vector<unsigned char> expected{'U', 0x2a};
  const auto bytes = rfl::ubjson::write(42);
  EXPECT_EQ(std::vector<unsigned char>(bytes.begin(), bytes.end()), expected);
  const auto result = rfl::ubjson::read<int>(expected);
  ASSERT_TRUE(result);
  EXPECT_EQ(result.value(), 42);

  std::stringstream stream;
  rfl::ubjson::write(42, stream);
  EXPECT_EQ(stream.str(), std::string(bytes.begin(), bytes.end()));
  const auto streamed = rfl::ubjson::read<int>(stream);
  ASSERT_TRUE(streamed);
  EXPECT_EQ(streamed.value(), 42);
}

TEST(ubjson, malformed_stream_reports_decoder_error) {
  std::stringstream stream(std::string(1, static_cast<char>(0xff)));
  const auto result = rfl::ubjson::read<int>(stream);
  ASSERT_FALSE(result);
  EXPECT_TRUE(result.error().what().starts_with("Could not parse UBJSON: "));
}
}  // namespace test_ubjson_stream_bridge
