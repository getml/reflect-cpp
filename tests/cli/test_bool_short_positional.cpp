#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/cli.hpp>
#include <string>

namespace test_bool_short_positional {

struct Config {
  rfl::Short<"v", bool> verbose;
  rfl::Positional<std::string> input_file;
};

TEST(cli, test_bool_short_does_not_consume_positional) {
  const char* args[] = {
      "program",
      "-v", "somefile.txt"
  };
  const auto result = rfl::cli::read<Config>(3, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_TRUE(result.value().verbose());
  EXPECT_EQ(result.value().input_file(), "somefile.txt");
}

TEST(cli, test_bool_short_explicit_true) {
  const char* args[] = {
      "program",
      "-v=true", "somefile.txt"
  };
  const auto result = rfl::cli::read<Config>(3, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_TRUE(result.value().verbose());
  EXPECT_EQ(result.value().input_file(), "somefile.txt");
}

TEST(cli, test_bool_short_explicit_false) {
  const char* args[] = {
      "program",
      "-v=false", "somefile.txt"
  };
  const auto result = rfl::cli::read<Config>(3, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_FALSE(result.value().verbose());
  EXPECT_EQ(result.value().input_file(), "somefile.txt");
}

struct PortConfig {
  rfl::Short<"p", int> port;
  rfl::Positional<std::string> input_file;
};

TEST(cli, test_int_short_still_consumes_value) {
  const char* args[] = {
      "program",
      "-p", "8080", "file.txt"
  };
  const auto result = rfl::cli::read<PortConfig>(4, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().port(), 8080);
  EXPECT_EQ(result.value().input_file(), "file.txt");
}

}  // namespace test_bool_short_positional
