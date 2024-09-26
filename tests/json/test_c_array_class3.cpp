#include <array>

#include "rfl.hpp"
#include "rfl/json.hpp"
#include "write_and_read.hpp"

namespace test_c_array_class3 {

using Test3 = std::array<int, 3>[3];

TEST(json, test_c_array_class3) {
#ifdef REFLECT_CPP_C_ARRAYS_OR_INHERITANCE
  Test3 test3 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  write_and_read(test3, "[[1,2,3],[4,5,6],[7,8,9]]");
#endif
}

}  // namespace test_c_array_class3
