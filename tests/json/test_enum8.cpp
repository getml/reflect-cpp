#include <gtest/gtest.h>

#include <cassert>
#include <rfl.hpp>
#include <rfl/json.hpp>

#include "write_and_read.hpp"

namespace test_enum8 {

enum class TestEnum : uint64_t {
  None = 0,
  Hello = 1 << 0,
  World = 1 << 1,
  HelloWorld = Hello | World
};

TestEnum operator|(TestEnum a, TestEnum b) noexcept {
  return static_cast<TestEnum>(static_cast<uint64_t>(a) |
                               static_cast<uint64_t>(b));
}

struct SomeClass {
  TestEnum e;
  TestEnum f;
  TestEnum g;
};

TEST(json, test_enum8) {
  SomeClass t{
      .e = TestEnum::None, .f = TestEnum::Hello, .g = TestEnum::HelloWorld};

  write_and_read(t, R"({"e":"0","f":"Hello","g":"Hello|World"})");
}

}  // namespace test_enum8
