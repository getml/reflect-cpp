#include <array>
#include <cassert>
#include <rfl/concepts.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_byte_container_concepts {

TEST(generic, test_byte_container_concepts) {
  // Test various container types
  std::vector<std::uint8_t> vec_uint8{1, 2, 3, 4};
  std::vector<char> vec_char{'a', 'b', 'c'};
  std::array<std::uint8_t, 4> arr_uint8{1, 2, 3, 4};
  std::string str_data{"hello"};
  std::string_view str_view{"world"};
  std::array<std::byte, 5> arr_byte{std::byte{5}, std::byte{4}, std::byte{3},
                                    std::byte{2}, std::byte{1}};
  std::span<std::byte> byte_span = arr_byte;

  // Test with concepts
  static_assert(rfl::concepts::ByteLike<std::uint8_t>);
  static_assert(rfl::concepts::ByteLike<char>);
  static_assert(rfl::concepts::ByteLike<unsigned char>);
  static_assert(rfl::concepts::ByteLike<std::byte>);
  static_assert(!rfl::concepts::ByteLike<int>);
  static_assert(!rfl::concepts::ByteLike<float>);

  static_assert(rfl::concepts::ContiguousByteContainer<decltype(vec_uint8)>);
  static_assert(rfl::concepts::ContiguousByteContainer<decltype(vec_char)>);
  static_assert(rfl::concepts::ContiguousByteContainer<decltype(arr_uint8)>);
  static_assert(rfl::concepts::ContiguousByteContainer<decltype(str_data)>);
  static_assert(rfl::concepts::ContiguousByteContainer<decltype(str_view)>);
  static_assert(rfl::concepts::ContiguousByteContainer<decltype(arr_byte)>);
  static_assert(rfl::concepts::ContiguousByteContainer<decltype(byte_span)>);

  static_assert(
      rfl::concepts::MutableContiguousByteContainer<decltype(vec_uint8)>);
  static_assert(
      rfl::concepts::MutableContiguousByteContainer<decltype(vec_char)>);
  static_assert(
      rfl::concepts::MutableContiguousByteContainer<decltype(arr_uint8)>);
  static_assert(
      rfl::concepts::MutableContiguousByteContainer<decltype(str_data)>);
  static_assert(
      !rfl::concepts::MutableContiguousByteContainer<decltype(str_view)>);
  static_assert(
      rfl::concepts::MutableContiguousByteContainer<decltype(arr_byte)>);
  static_assert(
      rfl::concepts::MutableContiguousByteContainer<decltype(byte_span)>);

  static_assert(
      rfl::concepts::BackInsertableByteContainer<decltype(vec_uint8)>);
  static_assert(rfl::concepts::BackInsertableByteContainer<decltype(vec_char)>);
  static_assert(
      !rfl::concepts::BackInsertableByteContainer<decltype(arr_uint8)>);
  static_assert(rfl::concepts::BackInsertableByteContainer<decltype(str_data)>);
  static_assert(
      !rfl::concepts::BackInsertableByteContainer<decltype(str_view)>);
  static_assert(
      !rfl::concepts::BackInsertableByteContainer<decltype(arr_byte)>);
  static_assert(
      !rfl::concepts::BackInsertableByteContainer<decltype(byte_span)>);
}

}  // namespace test_byte_container_concepts
