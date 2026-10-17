
#include <cstddef>

#include "write_and_read.hpp"

#include <test_rfl/rfl.capnproto.hpp>
#include <test_rfl/rfl.hpp>


namespace test_bytestring {

struct TestStruct {
  rfl::Bytestring bytestring;
};

TEST(capnproto, test_bytestring) {
  const auto test =
      TestStruct{.bytestring = rfl::Bytestring({std::byte{13}, std::byte{14},
                                                std::byte{15}, std::byte{16}})};

  write_and_read(test);
}
}  // namespace test_bytestring
