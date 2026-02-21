#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/cli.hpp>
#include <string>
#include <vector>

namespace test_vector {

struct Config {
  std::string name;
  std::vector<std::string> tags;
};

TEST(cli, test_vector) {
  const char* args[] = {
      "program",
      "--name=app",
      "--tags=dev,prod,staging"
  };
  const auto result = rfl::cli::read<Config>(3, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().name, "app");
  ASSERT_EQ(result.value().tags.size(), 3u);
  EXPECT_EQ(result.value().tags[0], "dev");
  EXPECT_EQ(result.value().tags[1], "prod");
  EXPECT_EQ(result.value().tags[2], "staging");
}

struct IntConfig {
  std::vector<int> values;
};

TEST(cli, test_vector_int) {
  const char* args[] = {
      "program",
      "--values=1,2,3"
  };
  const auto result = rfl::cli::read<IntConfig>(2, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  ASSERT_EQ(result.value().values.size(), 3u);
  EXPECT_EQ(result.value().values[0], 1);
  EXPECT_EQ(result.value().values[1], 2);
  EXPECT_EQ(result.value().values[2], 3);
}

TEST(cli, test_vector_skips_empty_elements) {
  const char* args[] = {
      "program",
      "--name=app",
      "--tags=dev,,prod"
  };
  const auto result = rfl::cli::read<Config>(3, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  ASSERT_EQ(result.value().tags.size(), 2u);
  EXPECT_EQ(result.value().tags[0], "dev");
  EXPECT_EQ(result.value().tags[1], "prod");
}

}  // namespace test_vector
