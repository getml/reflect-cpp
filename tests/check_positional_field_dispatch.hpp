#ifndef CHECK_POSITIONAL_FIELD_DISPATCH_HPP_
#define CHECK_POSITIONAL_FIELD_DISPATCH_HPP_
#include <gtest/gtest.h>

#include <rfl.hpp>
#include <string>
#include <utility>
namespace test_positional_field_dispatch {
struct Input {
  int first;
  std::string middle;
  int last;
};

class TrackedString {
 public:
  using ReflectionType = std::string;
  inline static int live = 0;

  explicit TrackedString(std::string value) : value_(std::move(value)) {
    ++live;
  }
  TrackedString(TrackedString&& other) : value_(std::move(other.value_)) {
    ++live;
  }
  ~TrackedString() { --live; }
  const std::string& reflection() const { return value_; }

 private:
  std::string value_;
};

struct Validated {
  rfl::Validator<int, rfl::Minimum<0>> first;
  TrackedString middle;
  rfl::Validator<int, rfl::Minimum<0>> last;
};

template <class Write, class Read>
void check_errors(Write write, Read read) {
  const auto bytes = write(Input{-1, "constructed between errors", -2});
  const auto result = read.template operator()<Validated>(bytes);
  ASSERT_FALSE(result);
  const std::string message = result.error().what();
  EXPECT_NE(message.find("Found 2 errors:"), std::string::npos);
  const auto first = message.find("Failed to parse field 'first'");
  const auto last = message.find("Failed to parse field 'last'");
  ASSERT_NE(first, std::string::npos);
  ASSERT_NE(last, std::string::npos);
  EXPECT_LT(first, last);
  EXPECT_EQ(TrackedString::live, 0);
}

template <class Write, class Read>
void check_truncated(Write write, Read read) {
  auto bytes = write(Input{1, "constructed before truncated field", 2});
  bytes.pop_back();
  const auto result = read.template operator()<Validated>(bytes);
  ASSERT_FALSE(result);
  EXPECT_NE(
      std::string(result.error().what()).find("Failed to parse field 'last'"),
      std::string::npos);
  EXPECT_EQ(TrackedString::live, 0);
}

struct Empty {};
struct WithEmpty {
  Empty empty;
  int value;
};
template <class Write, class Read>
void check_empty(Write write, Read read) {
  const auto result =
      read.template operator()<WithEmpty>(write(WithEmpty{{}, 42}));
  ASSERT_TRUE(result);
  EXPECT_EQ(result->value, 42);
}

template <class R>
struct RuntimeIndexReader {
  int* next;
  void read(int index, const typename R::InputVarType&) const {
    EXPECT_EQ(index, (*next)++);
  }
};

template <class R, int N>
struct IndexedReader : RuntimeIndexReader<R> {
  int* static_calls;
  static constexpr int size() { return N; }
  using RuntimeIndexReader<R>::read;
  template <int i>
  void read(std::integral_constant<int, i>,
            const typename R::InputVarType& var) const {
    ++*static_calls;
    RuntimeIndexReader<R>::read(i, var);
  }
};

template <int N, class R>
void check_count(const R& reader, const typename R::InputObjectType& obj,
                 int count) {
  int next = 0;
  int static_calls = 0;
  EXPECT_FALSE(
      reader.read_object(IndexedReader<R, N>{{&next}, &static_calls}, obj));
  EXPECT_EQ(next, count);
  EXPECT_EQ(static_calls, N == count ? count : 0);
}

template <class R>
void check_counts(const R& reader, const typename R::InputObjectType& obj,
                  int count) {
  check_count<0>(reader, obj, count);
  check_count<2>(reader, obj, count);
  check_count<3>(reader, obj, count);
  check_count<4>(reader, obj, count);
  int next = 0;
  EXPECT_FALSE(reader.read_object(RuntimeIndexReader<R>{&next}, obj));
  EXPECT_EQ(next, count);
}
}  // namespace test_positional_field_dispatch
#endif
