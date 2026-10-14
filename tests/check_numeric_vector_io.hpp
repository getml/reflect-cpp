#ifndef CHECK_NUMERIC_VECTOR_IO_HPP_
#define CHECK_NUMERIC_VECTOR_IO_HPP_

#include <gtest/gtest.h>

#include <cmath>
#include <cstdint>
#include <deque>
#include <limits>
#include <map>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace test_numeric_vector_io {

template <class T, class Write, class Read, class LegacyWrite, class LegacyRead>
void check_type(Write write, Read read, LegacyWrite legacy_write,
                LegacyRead legacy_read) {
  const std::vector<T> edge_values = {std::numeric_limits<T>::lowest(), T(0),
                                      T(1), std::numeric_limits<T>::max()};
  for (const auto count : {0, 1, 65}) {
    SCOPED_TRACE(count);
    std::vector<T> input;
    for (int i = 0; i < count; ++i) {
      input.push_back(edge_values[i % edge_values.size()]);
    }
    const auto old_bytes = legacy_write(input);
    const auto bytes = write(input);
    EXPECT_EQ(bytes, old_bytes);
    const auto decoded = read.template operator()<std::vector<T>>(old_bytes);
    ASSERT_TRUE(decoded) << decoded.error().what();
    EXPECT_EQ(*decoded, input);
    const auto old_decoded = legacy_read.template operator()<T>(bytes);
    ASSERT_TRUE(old_decoded) << old_decoded.error().what();
    EXPECT_EQ(*old_decoded, input);
    if (count != 0) {
      auto truncated = bytes;
      truncated.pop_back();
      EXPECT_FALSE(read.template operator()<std::vector<T>>(truncated));
    }
  }
  if constexpr (std::is_floating_point_v<T>) {
    const std::vector<T> input = {-T(0), T(1.25), -T(2.5),
                                  std::numeric_limits<T>::infinity(),
                                  -std::numeric_limits<T>::infinity()};
    EXPECT_EQ(write(input), legacy_write(input));
    const auto decoded =
        read.template operator()<std::vector<T>>(legacy_write(input));
    ASSERT_TRUE(decoded) << decoded.error().what();
    EXPECT_EQ(*decoded, input);
    EXPECT_EQ(write(*decoded), legacy_write(input));
    const std::vector<T> nan_input{std::numeric_limits<T>::quiet_NaN()};
    const auto nan_bytes = legacy_write(nan_input);
    EXPECT_EQ(write(nan_input), nan_bytes);
    const auto nan_decoded =
        read.template operator()<std::vector<T>>(nan_bytes);
    ASSERT_TRUE(nan_decoded) << nan_decoded.error().what();
    ASSERT_EQ(nan_decoded->size(), 1u);
    EXPECT_TRUE(std::isnan(nan_decoded->front()));
    EXPECT_EQ(write(*nan_decoded), nan_bytes);
  }
}

template <class Write, class Read, class LegacyWrite, class LegacyRead>
void check_types(Write write, Read read, LegacyWrite legacy_write,
                 LegacyRead legacy_read) {
  check_type<std::int8_t>(write, read, legacy_write, legacy_read);
  check_type<std::uint8_t>(write, read, legacy_write, legacy_read);
  check_type<std::int16_t>(write, read, legacy_write, legacy_read);
  check_type<std::uint16_t>(write, read, legacy_write, legacy_read);
  check_type<std::int32_t>(write, read, legacy_write, legacy_read);
  check_type<std::uint32_t>(write, read, legacy_write, legacy_read);
  check_type<std::int64_t>(write, read, legacy_write, legacy_read);
  check_type<std::uint64_t>(write, read, legacy_write, legacy_read);
  check_type<float>(write, read, legacy_write, legacy_read);
  check_type<double>(write, read, legacy_write, legacy_read);
  check_type<bool>(write, read, legacy_write, legacy_read);
}

template <class T>
using Vector = std::vector<T>;
template <class T>
using Deque = std::deque<T>;

template <template <class> class Sequence>
struct Nested {
  Sequence<std::uint64_t> ids;
  std::vector<Sequence<double>> rows;
  std::map<std::string, Sequence<float>> lookup;
  std::optional<Sequence<std::int32_t>> optional;
  std::variant<int, Sequence<std::int32_t>> choice;

  bool operator==(const Nested&) const = default;
};

struct VectorstringWrapper {
  std::vector<char> value;
  bool operator==(const VectorstringWrapper&) const = default;
};

template <class Write, class Read>
void check_nested(Write write, Read read) {
  const Nested<Vector> input{{0, 1, UINT64_MAX},
                             {{}, {1.25, -2.5}},
                             {{"values", {0.5f, -1.5f}}},
                             Vector<std::int32_t>{-1, 2},
                             Vector<std::int32_t>{3, -4}};
  const Nested<Deque> legacy{{0, 1, UINT64_MAX},
                             {{}, {1.25, -2.5}},
                             {{"values", {0.5f, -1.5f}}},
                             Deque<std::int32_t>{-1, 2},
                             Deque<std::int32_t>{3, -4}};
  const auto bytes = write(input);
  EXPECT_EQ(bytes, write(legacy));
  const auto decoded = read.template operator()<Nested<Vector>>(write(legacy));
  ASSERT_TRUE(decoded) << decoded.error().what();
  EXPECT_EQ(*decoded, input);
  const auto old_decoded = read.template operator()<Nested<Deque>>(bytes);
  ASSERT_TRUE(old_decoded) << old_decoded.error().what();
  EXPECT_EQ(*old_decoded, legacy);

  const VectorstringWrapper text{{'a', '\0', 'z'}};
  const auto text_decoded =
      read.template operator()<VectorstringWrapper>(write(text));
  ASSERT_TRUE(text_decoded) << text_decoded.error().what();
  EXPECT_EQ(*text_decoded, text);
}

}  // namespace test_numeric_vector_io

#endif
