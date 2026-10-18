#ifndef RFL_TESTS_CHECK_FAIL_FAST_HPP_
#define RFL_TESTS_CHECK_FAIL_FAST_HPP_

#include <gtest/gtest.h>

#include <cstdint>
#include <optional>
#include <rfl/Validator.hpp>
#include <rfl/comparisons.hpp>
#include <string>
#include <type_traits>
#include <vector>

namespace test_fail_fast {

struct Detail {
  std::string label;
  std::vector<int> values;
  std::optional<double> weight;
  bool operator==(const Detail&) const = default;
};

struct Record {
  const std::uint64_t id;
  std::vector<Detail> details;
  bool operator==(const Record&) const = default;
};

struct InvalidRecord {
  int first;
  int second;
};

struct ValidatedRecord {
  rfl::Validator<int, rfl::Minimum<0>> first;
  rfl::Validator<int, rfl::Minimum<0>> second;
};

template <class Write, class ReadDefault, class ReadFailFast>
void check_error_policy(Write write, ReadDefault read_default,
                        ReadFailFast read_fail_fast) {
  const auto bytes = write(InvalidRecord{-1, -2});
  const auto all_errors =
      read_default.template operator()<ValidatedRecord>(bytes);
  ASSERT_FALSE(all_errors);
  const auto all_message = std::string(all_errors.error().what());
  EXPECT_NE(all_message.find("Found 2 errors:"), std::string::npos);
  EXPECT_NE(all_message.find("field 'first'"), std::string::npos);
  EXPECT_NE(all_message.find("field 'second'"), std::string::npos);

  const auto first_error =
      read_fail_fast.template operator()<ValidatedRecord>(bytes);
  ASSERT_FALSE(first_error);
  const auto first_message = std::string(first_error.error().what());
  EXPECT_NE(first_message.find("field 'first'"), std::string::npos);
  EXPECT_EQ(first_message.find("field 'second'"), std::string::npos);
  EXPECT_EQ(first_message.find("Found 2 errors:"), std::string::npos);
}

// Read ordinary output with FailFast, including nontrivial and const fields.
template <class Write, class Read>
void check_round_trip(Write write, Read read) {
  const std::vector<Record> input{
      {17, {{"first", {1, 2, 3}, 0.5}, {"second", {}, std::nullopt}}},
      {42, {}}};
  const auto bytes = write(input);
  const auto result =
      read.template operator()<std::remove_cvref_t<decltype(input)>>(bytes);
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(*result, input);
  EXPECT_EQ(write(*result), bytes);
}

template <class Write, class Read>
void check_truncated(Write write, Read read) {
  const std::vector<Record> input{{17, {{"item", {1, 2}, 0.5}}}};
  const auto bytes = write(input);
  for (std::size_t size = 0; size < bytes.size(); ++size) {
    auto truncated = bytes;
    truncated.resize(size);
    const auto result =
        read.template operator()<std::vector<Record>>(truncated);
    EXPECT_FALSE(result) << "Accepted prefix of length " << size;
  }
}

}  // namespace test_fail_fast

#endif
