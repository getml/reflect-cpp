#include <string>

#include "write_and_read.hpp"
#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


namespace test_binary {

struct TestStruct {
  rfl::Binary<uint8_t> binary;
};

TEST(json, test_binary) {
  const auto test = TestStruct{.binary = 30};
  write_and_read(test, R"({"binary":"00011110"})");
}
}  // namespace test_binary
