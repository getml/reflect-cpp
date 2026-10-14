#ifndef CHECK_VECTOR_RESERVE_HPP_
#define CHECK_VECTOR_RESERVE_HPP_

#include <gtest/gtest.h>

#include <cstddef>
#include <vector>

namespace test_vector_reserve {

struct Value {
  using ReflectionType = float;

  inline static std::size_t moves = 0;

  explicit Value(float value) : value_(value) {}
  Value(const Value&) = delete;
  Value(Value&& other) noexcept : value_(other.value_) { ++moves; }

  const float& reflection() const { return value_; }

  bool operator==(const Value& other) const { return value_ == other.value_; }

 private:
  float value_;
};

struct Data {
  std::vector<Value> values;
};

template <class Write, class Read>
void check_moves(Write write, Read read) {
  std::size_t moves_per_element = 0;
  for (const auto size : {0, 1, 65}) {
    SCOPED_TRACE(size);
    Data input;
    for (int i = 0; i < size; ++i) {
      input.values.emplace_back(static_cast<float>(i) * 0.5f);
    }
    const auto data = write(input);
    Value::moves = 0;
    const auto result = read(data);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->values, input.values);
    if (size == 1) {
      moves_per_element = Value::moves;
      EXPECT_GT(moves_per_element, 0u);
    }
    // Growing the vector would add moves of previously parsed elements.
    EXPECT_EQ(Value::moves, size * moves_per_element);
  }
}

}  // namespace test_vector_reserve

#endif
