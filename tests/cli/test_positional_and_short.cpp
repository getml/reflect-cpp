#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/cli.hpp>
#include <string>

namespace test_positional_and_short {

struct Config {
  rfl::Positional<std::string> input_file;
  rfl::Short<"o", std::string> output_dir;
  rfl::Short<"v", bool> verbose;
  int count;
};

TEST(cli, test_positional_and_short_combined) {
  const char* args[] = {
      "program",
      "data.csv",
      "-o", "/tmp/out",
      "-v",
      "--count=10"
  };
  const auto result = rfl::cli::read<Config>(6, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().input_file(), "data.csv");
  EXPECT_EQ(result.value().output_dir(), "/tmp/out");
  EXPECT_TRUE(result.value().verbose());
  EXPECT_EQ(result.value().count, 10);
}

TEST(cli, test_positional_and_short_all_long) {
  const char* args[] = {
      "program",
      "--input-file=data.csv",
      "--output-dir=/tmp/out",
      "--verbose",
      "--count=10"
  };
  const auto result = rfl::cli::read<Config>(5, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().input_file(), "data.csv");
  EXPECT_EQ(result.value().output_dir(), "/tmp/out");
  EXPECT_TRUE(result.value().verbose());
  EXPECT_EQ(result.value().count, 10);
}

}  // namespace test_positional_and_short
