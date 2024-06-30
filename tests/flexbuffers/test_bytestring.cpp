#include <iostream>
#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_bytestring {

struct TestStruct {
  rfl::Bytestring bytestring;
  std::vector<rfl::Bytestring> bytestrings;
};

TEST(flexbuf, test_bytestring) {
  const auto bstr = rfl::Bytestring(
      {std::byte{13}, std::byte{14}, std::byte{15}, std::byte{16}});
  const auto test = TestStruct{
      .bytestring = bstr,
      .bytestrings = std::vector<rfl::Bytestring>({bstr, bstr, bstr})};
  write_and_read(test);
}
}  // namespace test_bytestring
