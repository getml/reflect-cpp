#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

// This example had led to a segfault on GCC 12.
// To make sure this won't happen again, we include
// it in our tests.
namespace test_segfault {

struct Data {
  std::string prop1;
  std::string prop2;
  std::string prop3;
  std::string prop4;
  std::string prop5;
  std::string prop6;
};

TEST(json, test_segfault) {
  const auto data = Data{};
  write_and_read(
      data,
      R"({"prop1":"","prop2":"","prop3":"","prop4":"","prop5":"","prop6":""})");
}
}  // namespace test_segfault
