#include <iostream>
#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_vectorstring {

struct TestStruct {
  rfl::Vectorstring vectorstring;
};

TEST(ubjson, test_vectorstring) {
  const auto test =
      TestStruct{.vectorstring = rfl::Vectorstring({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'})};

  write_and_read(test);
}
}  // namespace test_vectorstring