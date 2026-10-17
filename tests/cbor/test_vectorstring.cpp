
#include "write_and_read.hpp"
#include <test_rfl/rfl.cbor.hpp>
#include <test_rfl/rfl.hpp>


namespace test_vectorstring {

struct TestStruct {
  rfl::Vectorstring vectorstring;
};

TEST(cbor, test_vectorstring) {
  const auto test = TestStruct{
      .vectorstring = rfl::Vectorstring({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'})};

  write_and_read(test);
}
}  // namespace test_vectorstring
