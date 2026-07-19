#include <string>

#include "write_and_read.hpp"
import rfl;
import rfl.json;


namespace test_binary {

struct TestStruct {
  rfl::Binary<uint8_t> binary;
};

TEST(json, test_binary) {
  const auto test = TestStruct{.binary = 30};
  write_and_read(test, R"({"binary":"00011110"})");
}
}  // namespace test_binary
