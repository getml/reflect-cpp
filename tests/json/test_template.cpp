#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_template {

template <typename T>
struct A {
  int code;
  std::string msg;
  T data;
};

struct B {
  int some_val;
};

TEST(json, test_template) {
  const auto a = A<B>{.code = 0, .msg = "hello", .data = B{.some_val = 1}};

  write_and_read(a, R"({"code":0,"msg":"hello","data":{"some_val":1}})");
}

}  // namespace test_template
