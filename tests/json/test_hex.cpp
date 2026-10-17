#include <string>

#include "write_and_read.hpp"
#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


namespace test_hex {

struct TestStruct {
  rfl::Hex<int> hex;
};

TEST(json, test_hex) {
  const auto test = TestStruct{.hex = 30};
  write_and_read(test, R"({"hex":"1e"})");
}
}  // namespace test_hex
