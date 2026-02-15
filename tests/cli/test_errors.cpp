#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/cli.hpp>
#include <string>

namespace test_errors {

struct Config {
  std::string name;
  int port;
};

TEST(cli, test_error_invalid_arg_format) {
  const char* args[] = {"program", "not-a-flag"};
  const auto result = rfl::cli::read<Config>(2, const_cast<char**>(args));
  EXPECT_FALSE(result);
}

TEST(cli, test_error_invalid_int) {
  const char* args[] = {"program", "--name=test", "--port=abc"};
  const auto result = rfl::cli::read<Config>(3, const_cast<char**>(args));
  EXPECT_FALSE(result);
}

TEST(cli, test_error_duplicate_key) {
  const char* args[] = {"program", "--name=a", "--name=b", "--port=1"};
  const auto result = rfl::cli::read<Config>(4, const_cast<char**>(args));
  EXPECT_FALSE(result);
}

}  // namespace test_errors
