#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_positional_and_short_json {

struct PositionalConfig {
  rfl::Positional<std::string> input_file;
  rfl::Positional<int> count;
};

TEST(json, test_positional_write_and_read) {
  write_and_read(
      PositionalConfig{"data.csv", 42},
      R"({"input_file":"data.csv","count":42})");
}

struct ShortConfig {
  rfl::Short<"p", int> port;
  rfl::Short<"v", bool> verbose;
  std::string host;
};

TEST(json, test_short_write_and_read) {
  write_and_read(
      ShortConfig{8080, true, "localhost"},
      R"({"port":8080,"verbose":true,"host":"localhost"})");
}

struct CombinedConfig {
  rfl::Positional<std::string> input_file;
  rfl::Short<"o", std::string> output_dir;
  rfl::Short<"v", bool> verbose;
  int count;
  std::vector<std::string> tags;
};

TEST(json, test_positional_and_short_combined_write_and_read) {
  write_and_read(
      CombinedConfig{"data.csv", "/tmp/out", true, 10, {"dev", "prod"}},
      R"({"input_file":"data.csv","output_dir":"/tmp/out","verbose":true,"count":10,"tags":["dev","prod"]})");
}

TEST(json, test_positional_read_from_json) {
  const auto result = rfl::json::read<PositionalConfig>(
      R"({"input_file":"test.txt","count":7})");
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().input_file(), "test.txt");
  EXPECT_EQ(result.value().count(), 7);
}

TEST(json, test_short_read_from_json) {
  const auto result = rfl::json::read<ShortConfig>(
      R"({"port":3000,"verbose":false,"host":"example.com"})");
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().port(), 3000);
  EXPECT_FALSE(result.value().verbose());
  EXPECT_EQ(result.value().host, "example.com");
}

}  // namespace test_positional_and_short_json
