#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/cli.hpp>
#include <string>

namespace test_positional {

struct Config {
  rfl::Positional<std::string> input_file;
  rfl::Positional<std::string> output_file;
  bool verbose;
};

TEST(cli, test_positional_basic) {
  const char* args[] = {
      "program",
      "in.txt",
      "out.txt",
      "--verbose"
  };
  const auto result = rfl::cli::read<Config>(4, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().input_file(), "in.txt");
  EXPECT_EQ(result.value().output_file(), "out.txt");
  EXPECT_TRUE(result.value().verbose);
}

TEST(cli, test_positional_as_named) {
  const char* args[] = {
      "program",
      "--input-file=in.txt",
      "--output-file=out.txt",
      "--verbose"
  };
  const auto result = rfl::cli::read<Config>(4, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().input_file(), "in.txt");
  EXPECT_EQ(result.value().output_file(), "out.txt");
}

TEST(cli, test_positional_too_many) {
  const char* args[] = {
      "program",
      "in.txt",
      "out.txt",
      "extra.txt"
  };
  const auto result = rfl::cli::read<Config>(4, const_cast<char**>(args));
  ASSERT_FALSE(result);
}

struct SimpleConfig {
  rfl::Positional<std::string> input_file;
  rfl::Positional<std::string> output_file;
};

TEST(cli, test_positional_double_dash_separator) {
  const char* args[] = {
      "program",
      "--",
      "--not-a-flag",
      "out.txt"
  };
  const auto result =
      rfl::cli::read<SimpleConfig>(4, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().input_file(), "--not-a-flag");
  EXPECT_EQ(result.value().output_file(), "out.txt");
}

TEST(cli, test_positional_conflict_with_named) {
  const char* args[] = {
      "program",
      "in.txt",
      "--input-file=other.txt"
  };
  const auto result = rfl::cli::read<Config>(3, const_cast<char**>(args));
  ASSERT_FALSE(result);
}

struct OptionalPositional {
  rfl::Positional<std::string> required_file;
  rfl::Positional<std::optional<std::string>> optional_file;
};

TEST(cli, test_positional_optional_missing) {
  const char* args[] = {
      "program",
      "in.txt"
  };
  const auto result =
      rfl::cli::read<OptionalPositional>(2, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().required_file(), "in.txt");
  EXPECT_FALSE(result.value().optional_file().has_value());
}

}  // namespace test_positional
