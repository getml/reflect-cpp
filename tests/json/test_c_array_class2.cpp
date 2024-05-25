#include <iostream>
#include <source_location>
#include <string>

#include "rfl.hpp"
#include "rfl/json.hpp"
#include "write_and_read.hpp"

namespace test_c_array_class2 {

struct Test2 {
  int multi_dimension_arr[2][2][2][2];
  std::string s;
};

TEST(json, test_c_array_class2) {
  Test2 test2;
  for (int i = 0; i < 1 << 4; i++) {
    test2.multi_dimension_arr[i >> 3 & 1][i >> 2 & 1][i >> 1 & 1][i >> 0 & 1] =
        i;
  }
  test2.s = "Hello, world!";

  write_and_read(
      test2,
      R"({"multi_dimension_arr":[[[[0,1],[2,3]],[[4,5],[6,7]]],[[[8,9],[10,11]],[[12,13],[14,15]]]],"s":"Hello, world!"})");
}

}  // namespace test_c_array_class2
