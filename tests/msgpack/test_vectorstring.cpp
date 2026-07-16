
#include "write_and_read.hpp"
import rfl.msgpack;
import rfl;


namespace test_vectorstring {

struct TestStruct {
  rfl::Vectorstring vectorstring;
};

TEST(msgpack, test_vectorstring) {
  const auto test =
      TestStruct{.vectorstring = rfl::Vectorstring({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'})};

  write_and_read(test);
}
}  // namespace test_vectorstring
