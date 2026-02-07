#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/cli.hpp>
#include <string>

namespace test_rename {

struct Config {
  rfl::Rename<"output-dir", std::string> output_dir;
  int count;
};

TEST(cli, test_rename) {
  const char* args[] = {
      "program",
      "--output-dir=/tmp/out",
      "--count=5"
  };
  const auto result = rfl::cli::read<Config>(3, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().output_dir(), "/tmp/out");
  EXPECT_EQ(result.value().count, 5);
}

}  // namespace test_rename
