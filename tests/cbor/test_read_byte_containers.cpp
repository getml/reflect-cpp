#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <rfl/cbor.hpp>

// Make sure things still compile when
// rfl.hpp is included after rfl/cbor.hpp.
#include <rfl.hpp>

namespace test_read_byte_containers {

struct TestBox {
  int length;
  int width;
  int height;
};

TEST(cbor, test_read_from_byte_view) {
  TestBox b = {
      .length = 1,
      .width = 2,
      .height = 3,
  };

  // TODO: Write directly into desired container, once rfl::cbor::write supports
  // it.
  std::vector<char> rfl_buffer = rfl::cbor::write(b);

  std::array<std::byte, 64> my_buffer;
  std::transform(rfl_buffer.begin(), rfl_buffer.end(), my_buffer.begin(),
                 [](char c) { return static_cast<std::byte>(c); });

  std::basic_string_view<std::byte> byte_view(my_buffer.data(),
                                              rfl_buffer.size());

  auto result = rfl::cbor::read<TestBox>(byte_view);
  EXPECT_TRUE(result);
  EXPECT_EQ(result->length, 1);
  EXPECT_EQ(result->width, 2);
  EXPECT_EQ(result->height, 3);
}

TEST(cbor, test_read_from_uint8_array) {
  TestBox b = {
      .length = 4,
      .width = 5,
      .height = 6,
  };

  // TODO: Write directly into desired container, once rfl::cbor::write supports
  // it.
  std::vector<char> rfl_buffer = rfl::cbor::write(b);

  std::array<std::uint8_t, 64> my_buffer;
  std::transform(rfl_buffer.begin(), rfl_buffer.end(), my_buffer.begin(),
                 [](char c) { return static_cast<std::uint8_t>(c); });

  auto result = rfl::cbor::read<TestBox>(my_buffer);
  EXPECT_TRUE(result);
  EXPECT_EQ(result->length, 4);
  EXPECT_EQ(result->width, 5);
  EXPECT_EQ(result->height, 6);
}

}  // namespace test_read_byte_containers
