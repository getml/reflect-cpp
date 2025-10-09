#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_vectorstring {

struct TestStruct {
  rfl::Vectorstring vectorstring;
  std::vector<rfl::Vectorstring> vectorstrings;
};

TEST(bson, test_vectorstring) {
  const auto vstr = rfl::Vectorstring(
      {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'});
  const auto test = TestStruct{
      .vectorstring = vstr,
      .vectorstrings = std::vector<rfl::Vectorstring>({vstr, vstr, vstr})};
  write_and_read(test);
}
}  // namespace test_vectorstring
