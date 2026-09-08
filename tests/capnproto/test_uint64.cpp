#include <rfl.hpp>
#include <cstdint>
#include <climits>
#include <string>

#include "write_and_read.hpp"

namespace test_uint64 {

struct UnsignedStruct {
  uint64_t x = UINT64_MAX;
  uint32_t y = UINT32_MAX;
  uint16_t z = UINT16_MAX;
  uint8_t w = UINT8_MAX;
};

TEST(capnproto, test_uint64) {
  const auto s = UnsignedStruct{};
  write_and_read(s);
}

TEST(capnproto, test_uint64_specific_values) {
  const auto s = UnsignedStruct{
      .x = 0, .y = 0, .z = 0, .w = 0};
  write_and_read(s);
}

TEST(capnproto, test_uint64_max_values) {
  const auto s = UnsignedStruct{
      .x = UINT64_MAX, .y = UINT32_MAX, .z = UINT16_MAX, .w = UINT8_MAX};
  write_and_read(s);
}

}  // namespace test_uint64
