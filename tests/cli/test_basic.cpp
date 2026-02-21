#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/cli.hpp>
#include <string>

namespace test_basic {

struct Config {
  std::string host;
  int port;
  double rate;
  bool verbose;
};

TEST(cli, test_basic) {
  const char* args[] = {
      "program",
      "--host=localhost",
      "--port=8080",
      "--rate=1.5",
      "--verbose"
  };
  const auto result = rfl::cli::read<Config>(5, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().host, "localhost");
  EXPECT_EQ(result.value().port, 8080);
  EXPECT_DOUBLE_EQ(result.value().rate, 1.5);
  EXPECT_TRUE(result.value().verbose);
}

TEST(cli, test_basic_bool_explicit_true) {
  const char* args[] = {"program", "--host=x", "--port=1", "--rate=0", "--verbose=true"};
  const auto result = rfl::cli::read<Config>(5, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_TRUE(result.value().verbose);
}

TEST(cli, test_basic_bool_false) {
  const char* args[] = {"program", "--host=x", "--port=1", "--rate=0", "--verbose=false"};
  const auto result = rfl::cli::read<Config>(5, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_FALSE(result.value().verbose);
}

}  // namespace test_basic
