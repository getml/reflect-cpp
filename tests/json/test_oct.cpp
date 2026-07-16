#include <string>

#include "write_and_read.hpp"
import rfl;
import rfl.json;


namespace test_oct {

struct TestStruct {
  rfl::Oct<int> oct;
};

TEST(json, test_oct) {
  const auto test = TestStruct{.oct = 30};
  write_and_read(test, R"({"oct":"36"})");
}
}  // namespace test_oct
