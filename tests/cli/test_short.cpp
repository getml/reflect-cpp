#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/cli.hpp>
#include <string>

namespace test_short {

struct Config {
  std::string host;
  rfl::Short<"p", int> port;
  rfl::Short<"v", bool> verbose;
};

TEST(cli, test_short_basic) {
  const char* args[] = {
      "program",
      "--host=localhost",
      "-p", "8080",
      "-v"
  };
  const auto result = rfl::cli::read<Config>(5, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().host, "localhost");
  EXPECT_EQ(result.value().port(), 8080);
  EXPECT_TRUE(result.value().verbose());
}

TEST(cli, test_short_equals_syntax) {
  const char* args[] = {
      "program",
      "--host=localhost",
      "-p=8080",
      "-v"
  };
  const auto result = rfl::cli::read<Config>(4, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().port(), 8080);
}

TEST(cli, test_short_as_long) {
  const char* args[] = {
      "program",
      "--host=localhost",
      "--port=8080",
      "--verbose"
  };
  const auto result = rfl::cli::read<Config>(4, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().port(), 8080);
  EXPECT_TRUE(result.value().verbose());
}

TEST(cli, test_short_unknown) {
  const char* args[] = {
      "program",
      "--host=localhost",
      "-x", "foo"
  };
  const auto result = rfl::cli::read<Config>(4, const_cast<char**>(args));
  ASSERT_FALSE(result);
}

TEST(cli, test_short_conflict_with_long) {
  const char* args[] = {
      "program",
      "--host=localhost",
      "--port=8080",
      "-p", "9090"
  };
  const auto result = rfl::cli::read<Config>(5, const_cast<char**>(args));
  ASSERT_FALSE(result);
}

struct NegativeConfig {
  std::string host;
  rfl::Short<"p", int> port;
};

TEST(cli, test_short_negative_value) {
  const char* args[] = {
      "program",
      "--host=localhost",
      "-p", "-42"
  };
  const auto result = rfl::cli::read<NegativeConfig>(
      4, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().port(), -42);
}

struct FloatConfig {
  rfl::Short<"t", double> threshold;
};

TEST(cli, test_short_negative_float) {
  const char* args[] = {
      "program",
      "-t", "-3.14"
  };
  const auto result = rfl::cli::read<FloatConfig>(
      3, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_DOUBLE_EQ(result.value().threshold(), -3.14);
}

}  // namespace test_short
