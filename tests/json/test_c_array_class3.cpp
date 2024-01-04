#include "test_c_array_class3.hpp"

#include <source_location>
#include <array>

#include "rfl.hpp"
#include "rfl/json.hpp"
#include "write_and_read.hpp"

namespace test_c_array_class3 {

using Test3 = std::array<int, 3>[3];

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  Test3 test3 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  write_and_read(test3, "[[1,2,3],[4,5,6],[7,8,9]]");
}

}  // namespace test_c_array
