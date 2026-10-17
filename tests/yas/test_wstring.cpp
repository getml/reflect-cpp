#include <cassert>
#include <string>

#include "write_and_read.hpp"
#include <test_rfl/rfl.yas.hpp>
#include <test_rfl/rfl.hpp>


struct TestStruct {
  std::string theNormalString;
  std::wstring theWiderString;
};

namespace test_wstring {
TEST(yas, test_wstring) {
  const auto test = TestStruct{.theNormalString = "The normal string",
                               .theWiderString = L"The wider string"};

  write_and_read(test);
}
}  // namespace test_wstring
