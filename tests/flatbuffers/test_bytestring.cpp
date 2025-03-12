#include <cstdint>
#include <iostream>
#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_bytestring {

struct TestStruct {
  //  rfl::Bytestring bytestring;
  std::vector<std::byte> bytestring;
};

TEST(flatbuffers, test_bytestring) {
  std::cout << rfl::flatbuf::to_schema<TestStruct>().value() << std::endl;

  const auto test =
      TestStruct{.bytestring = rfl::Bytestring({std::byte{13}, std::byte{14},
                                                std::byte{15}, std::byte{16}})};

  write_and_read(test);
}
}  // namespace test_bytestring
