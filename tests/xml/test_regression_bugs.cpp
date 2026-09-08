#include <gtest/gtest.h>

#include <cstdint>
#include <limits>
#include <string>

#include <rfl.hpp>
#include <rfl/xml.hpp>

// xml::Reader::to_basic_type uses std::stoi for all integral types
// File: include/rfl/xml/Reader.hpp:99-105
// std::stoi returns int (32-bit). For int64_t/uint64_t fields, values
// exceeding INT_MAX will either throw std::out_of_range or be silently
// truncated via static_cast<T>(stoi(str)).
namespace test_xml_reader_int64 {

struct WithInt64 {
  int64_t big_number;
};

struct WithUint64 {
  uint64_t big_unsigned;
};

TEST(xml_regression, read_int64_above_int32_max) {
  const int64_t value = static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1;
  const auto xml = std::string(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<WithInt64><big_number>") +
      std::to_string(value) +
      std::string("</big_number></WithInt64>");

  const auto res = rfl::xml::read<WithInt64>(xml);
  ASSERT_TRUE(res) << "Reading int64 value " << value
                   << " should succeed. Error: " << res.error().what();
  EXPECT_EQ(res.value().big_number, value)
      << "int64 value above INT32_MAX was truncated or parsed incorrectly";
}

TEST(xml_regression, read_int64_large_negative) {
  const int64_t value = static_cast<int64_t>(std::numeric_limits<int32_t>::min()) - 1;
  const auto xml = std::string(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<WithInt64><big_number>") +
      std::to_string(value) +
      std::string("</big_number></WithInt64>");

  const auto res = rfl::xml::read<WithInt64>(xml);
  ASSERT_TRUE(res) << "Reading int64 value " << value
                   << " should succeed. Error: " << res.error().what();
  EXPECT_EQ(res.value().big_number, value)
      << "int64 value below INT32_MIN was truncated or parsed incorrectly";
}

TEST(xml_regression, read_uint64_large_value) {
  const uint64_t value = static_cast<uint64_t>(std::numeric_limits<int32_t>::max()) + 100;
  const auto xml = std::string(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<WithUint64><big_unsigned>") +
      std::to_string(value) +
      std::string("</big_unsigned></WithUint64>");

  const auto res = rfl::xml::read<WithUint64>(xml);
  ASSERT_TRUE(res) << "Reading uint64 value " << value
                   << " should succeed. Error: " << res.error().what();
  EXPECT_EQ(res.value().big_unsigned, value)
      << "uint64 value was truncated by stoi (32-bit)";
}

}  // namespace test_xml_reader_int64

// xml::Reader narrowing: stoull/stoll + static_cast<T> silently truncates
// for types narrower than 64-bit (e.g. int16_t, uint16_t).
namespace test_xml_reader_narrowing {

struct WithInt16 {
  int16_t value;
};

struct WithUint16 {
  uint16_t value;
};

TEST(xml_regression, read_rejects_out_of_range_for_int16) {
  const auto xml = std::string(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<WithInt16><value>99999</value></WithInt16>");
  const auto res = rfl::xml::read<WithInt16>(xml);
  EXPECT_FALSE(res) << "99999 should be rejected for int16_t field";
}

TEST(xml_regression, read_rejects_negative_for_uint16) {
  const auto xml = std::string(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<WithUint16><value>-1</value></WithUint16>");
  const auto res = rfl::xml::read<WithUint16>(xml);
  EXPECT_FALSE(res) << "-1 should be rejected for uint16_t field";
}

TEST(xml_regression, read_rejects_large_negative_for_int16) {
  const auto xml = std::string(
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<WithInt16><value>-99999</value></WithInt16>");
  const auto res = rfl::xml::read<WithInt16>(xml);
  EXPECT_FALSE(res) << "-99999 should be rejected for int16_t field";
}

}  // namespace test_xml_reader_narrowing
