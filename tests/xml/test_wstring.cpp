#include "test_wstring.hpp"

#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

struct Test {
  std::string theNormalString;
  std::wstring theWiderString;
};

namespace test_wstring {
void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const Test test = Test{.theNormalString = "The normal string",
                         .theWiderString = L"The wider string"};

  write_and_read<"root">(test);
}
}  // namespace test_wstring
