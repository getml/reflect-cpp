#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <tuple>
#include <vector>

#include "write_and_read.hpp"

namespace test_leak_tup {

class TestLeak {
 public:
  inline static int create_count = 0;
  inline static int delete_count = 0;

  using ReflectionType = std::string;

  TestLeak(const std::string& _str) : moved_(false), str_(_str) {
    ++create_count;
  }

  TestLeak(const TestLeak& _other) : moved_(false), str_(_other.str_) {
    ++create_count;
  }

  TestLeak(TestLeak&& _other) noexcept
      : moved_(false), str_(std::move(_other.str_)) {
    _other.moved_ = true;
    ++create_count;
    ++delete_count;
  }

  ~TestLeak() {
    if (!moved_) {
      ++delete_count;
    }
  }

  TestLeak& operator=(const TestLeak& _other) {
    str_ = _other.str_;
    return *this;
  }

  TestLeak& operator=(TestLeak&& _other) noexcept {
    str_ = std::move(_other.str_);
    _other.moved_ = true;
    ++delete_count;
    return *this;
  }

  const std::string& reflection() { return str_; }

 private:
  bool moved_;
  std::string str_;
};

TEST(json, test_leak_tup) {
  {
    const auto r =
        rfl::json::read<std::tuple<std::string, std::string, const TestLeak>>(
            R"(["Homer","Simpson","test"])");
  }
  EXPECT_EQ(TestLeak::delete_count, TestLeak::create_count);
}
}  // namespace test_leak_tup
