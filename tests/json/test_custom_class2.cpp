#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <stdexcept>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_custom_class2 {

struct FiveDigitCode {
  using ReflectionType = int;

  FiveDigitCode(const int _code) : code(_code) {
    if (_code < 10000) {
      throw std::runtime_error("Less than five digits.");
    }
    if (_code > 99999) {
      throw std::runtime_error("More than five digits.");
    }
  }

  ReflectionType reflection() const { return code; };

 private:
  int code;
};

TEST(json, test_custom_class2) {
  const auto result = rfl::json::read<FiveDigitCode>("123");

  EXPECT_TRUE(!result && true);

  const auto c = FiveDigitCode(12345);

  write_and_read(c, "12345");
}
}  // namespace test_custom_class2
