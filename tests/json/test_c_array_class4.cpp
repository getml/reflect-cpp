#include <array>
#include <source_location>

#include "rfl.hpp"
#include "rfl/json.hpp"
#include "write_and_read.hpp"

namespace test_c_array_class4 {

struct Test4 {
  int a[3];
  int b[3];
  int c[2][2];
};

TEST(json, test_c_array_class4) {
  Test4 test4 = {.a = {1, 2, 3}, .b = {4, 5, 6}, .c = {{7, 8}, {9, 10}}};

  write_and_read(test4, R"({"a":[1,2,3],"b":[4,5,6],"c":[[7,8],[9,10]]})");
}

}  // namespace test_c_array_class4
