#include <vector>

#include <cstddef>

#include "write_and_read.hpp"

#include <test_rfl/rfl.bson.hpp>
#include <test_rfl/rfl.hpp>


namespace test_bytestring {

struct TestStruct {
  rfl::Bytestring bytestring;
  std::vector<rfl::Bytestring> bytestrings;
};

TEST(bson, test_bytestring) {
  const auto bstr = rfl::Bytestring(
      {std::byte{13}, std::byte{14}, std::byte{15}, std::byte{16}});
  const auto test = TestStruct{
      .bytestring = bstr,
      .bytestrings = std::vector<rfl::Bytestring>({bstr, bstr, bstr})};
  write_and_read(test);
}
}  // namespace test_bytestring
