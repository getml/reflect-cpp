#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_oct {

struct TestStruct {
  rfl::Oct<int> oct;
};

TEST(json, test_oct) {
  const auto test = TestStruct{.oct = 30};
  write_and_read(test, R"({"oct":"36"})");
}
}  // namespace test_oct
