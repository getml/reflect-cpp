#include <array>
#include <source_location>

#include "rfl.hpp"
#include "rfl/json.hpp"
#include "write_and_read.hpp"

namespace test_c_array_class5 {

struct Test5 {
  int a[3];
  int b[3];
  int c[2][2];
};

TEST(json, test_c_array_class5) {
  Test5 t1 = {.a = {1, 2, 3}, .b = {4, 5, 6}, .c = {{7, 8}, {9, 10}}};

  const auto t2 = rfl::replace(t1, rfl::make_field<"b", int[3]>({1, 2, 3}));

  write_and_read(t2, R"({"a":[1,2,3],"b":[1,2,3],"c":[[7,8],[9,10]]})");
}

}  // namespace test_c_array_class5
