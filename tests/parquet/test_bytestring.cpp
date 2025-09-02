#include <iostream>
#include <rfl/parquet.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_bytestring {

struct TestStruct {
  rfl::Bytestring bytestring;
};

TEST(parquet, test_bytestring) {
  const auto test_struct =
      TestStruct{.bytestring = rfl::Bytestring({std::byte{13}, std::byte{14},
                                                std::byte{15}, std::byte{16}})};

  const auto test_structs = std::vector<TestStruct>(
      {test_struct, test_struct, test_struct, test_struct});

  write_and_read(test_structs);
}
}  // namespace test_bytestring
