#include <cassert>
#include <rfl.hpp>
#include <string>

#include "write_and_read.hpp"

struct TestStruct {
  std::string theNormalString;
  std::wstring theWiderString;
};

namespace test_wstring {
TEST(xml, test_wstring) {
  const auto test = TestStruct{.theNormalString = "The normal string",
                               .theWiderString = L"The wider string"};

  write_and_read(test);
}
}  // namespace test_wstring
