#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_float_and_int {

struct float_struct {
  float i;
};

struct double_struct {
  double i;
};

struct int_struct {
  int i;
};

struct long_struct {
  long i;
};

TEST(avro, test_float_and_int) {
  float_struct test_float_struct{.i = 0.1};

  double_struct test_double_struct{.i = 0.1};

  int_struct test_int_struct{.i = 1};

  long_struct test_long_struct{.i = 1};

  write_and_read_with_json(test_float_struct);
  write_and_read_with_json(test_double_struct);
  write_and_read_with_json(test_int_struct);
  write_and_read_with_json(test_long_struct);
}
}  // namespace test_float_and_int
