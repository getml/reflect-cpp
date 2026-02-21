#include <gtest/gtest.h>

#include <atomic>
#include <cstdint>
#include <rfl.hpp>
#include <rfl/json.hpp>

namespace test_atomic_negative {

struct TestObject {
  std::uint64_t number{0};
};

struct AtomicTestObject {
  std::atomic<std::uint64_t> number{0};
};

TEST(json, test_atomic_negative) {
  auto req1 = rfl::json::read<TestObject>(R"({"number": -1})");
  EXPECT_FALSE(req1.has_value());

  auto req2 = rfl::json::read<rfl::Ref<AtomicTestObject>>(R"({"number": -1})");
  EXPECT_FALSE(req2.has_value());
}

}  // namespace test_atomic_negative
