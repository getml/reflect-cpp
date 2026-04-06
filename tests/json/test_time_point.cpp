#include <gtest/gtest.h>

#include <chrono>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

namespace test_time_point {

struct Event {
  std::string name;
  std::chrono::system_clock::time_point created_at;
};

TEST(json, test_time_point_round_trip) {
  const auto now = std::chrono::system_clock::now();
  const auto event = Event{.name = "deploy", .created_at = now};

  const auto json = rfl::json::write(event);
  const auto result = rfl::json::read<Event>(json);

  ASSERT_TRUE(result && true) << result.error().what();
  EXPECT_EQ(result.value().name, "deploy");

  // Compare at the system clock's native resolution.
  const auto expected =
      std::chrono::time_point_cast<std::chrono::system_clock::duration>(now);
  const auto actual =
      std::chrono::time_point_cast<std::chrono::system_clock::duration>(
          result.value().created_at);
  EXPECT_EQ(expected, actual);
}

TEST(json, test_time_point_format) {
  // 2024-01-15T12:00:00.123456Z
  const auto epoch = std::chrono::system_clock::from_time_t(1705320000);
  const auto tp = epoch + std::chrono::microseconds(123456);
  const auto event = Event{.name = "test", .created_at = tp};

  const auto json = rfl::json::write(event);
  EXPECT_TRUE(json.find(".123456Z") != std::string::npos) << "Got: " << json;

  // Verify round-trip preserves the exact microseconds.
  const auto result = rfl::json::read<Event>(json);
  ASSERT_TRUE(result && true) << result.error().what();
  EXPECT_EQ(std::chrono::time_point_cast<std::chrono::microseconds>(tp),
            std::chrono::time_point_cast<std::chrono::microseconds>(
                result.value().created_at));
}

TEST(json, test_time_point_no_fractional) {
  const auto tp = std::chrono::system_clock::from_time_t(1705320000);
  const auto event = Event{.name = "test", .created_at = tp};

  const auto json = rfl::json::write(event);
  // Should not have fractional seconds.
  EXPECT_TRUE(json.find("\"Z\"") == std::string::npos)
      << "Should not be quoted Z";
  EXPECT_TRUE(json.find(".") == std::string::npos)
      << "Should not have fractional part. Got: " << json;
}

TEST(json, test_time_point_parse_various_precisions) {
  // Milliseconds.
  auto r1 = rfl::json::read<Event>(
      R"({"name":"a","created_at":"2024-01-15T10:30:00.123Z"})");
  ASSERT_TRUE(r1 && true) << r1.error().what();

  // Nanoseconds.
  auto r2 = rfl::json::read<Event>(
      R"({"name":"b","created_at":"2024-01-15T10:30:00.123456789Z"})");
  ASSERT_TRUE(r2 && true) << r2.error().what();

  // No fractional part.
  auto r3 = rfl::json::read<Event>(
      R"({"name":"c","created_at":"2024-01-15T10:30:00Z"})");
  ASSERT_TRUE(r3 && true) << r3.error().what();
}

TEST(json, test_time_point_reject_invalid_suffix) {
  // Trailing garbage should fail.
  auto r1 = rfl::json::read<Event>(
      R"({"name":"a","created_at":"2024-01-15T10:30:00Invalid"})");
  EXPECT_FALSE(r1 && true);

  // No Z is accepted (end of string).
  auto r2 = rfl::json::read<Event>(
      R"({"name":"b","created_at":"2024-01-15T10:30:00"})");
  EXPECT_TRUE(r2 && true) << r2.error().what();
}

TEST(json, test_time_point_timezone_offset) {
  // +05:30 means 5h30m ahead of UTC, so 10:30+05:30 = 05:00Z.
  auto r1 = rfl::json::read<Event>(
      R"({"name":"a","created_at":"2024-01-15T10:30:00+05:30"})");
  ASSERT_TRUE(r1 && true) << r1.error().what();

  auto r_utc = rfl::json::read<Event>(
      R"({"name":"a","created_at":"2024-01-15T05:00:00Z"})");
  ASSERT_TRUE(r_utc && true) << r_utc.error().what();

  EXPECT_EQ(
      std::chrono::time_point_cast<std::chrono::seconds>(r1.value().created_at),
      std::chrono::time_point_cast<std::chrono::seconds>(
          r_utc.value().created_at));

  // Negative offset: -08:00 means 8h behind UTC, so 02:00-08:00 = 10:00Z.
  auto r2 = rfl::json::read<Event>(
      R"({"name":"b","created_at":"2024-01-15T02:00:00-08:00"})");
  ASSERT_TRUE(r2 && true) << r2.error().what();

  auto r_utc2 = rfl::json::read<Event>(
      R"({"name":"b","created_at":"2024-01-15T10:00:00Z"})");
  ASSERT_TRUE(r_utc2 && true) << r_utc2.error().what();

  EXPECT_EQ(
      std::chrono::time_point_cast<std::chrono::seconds>(r2.value().created_at),
      std::chrono::time_point_cast<std::chrono::seconds>(
          r_utc2.value().created_at));

  // Offset with fractional seconds.
  auto r3 = rfl::json::read<Event>(
      R"({"name":"c","created_at":"2024-01-15T10:30:00.5+05:30"})");
  ASSERT_TRUE(r3 && true) << r3.error().what();
}

}  // namespace test_time_point
