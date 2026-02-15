#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/cli.hpp>
#include <optional>
#include <string>

namespace test_optional {

struct Config {
  std::string name;
  std::optional<int> port;
  std::optional<std::string> label;
};

TEST(cli, test_optional_present) {
  const char* args[] = {
      "program",
      "--name=test",
      "--port=3000",
      "--label=dev"
  };
  const auto result = rfl::cli::read<Config>(4, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().name, "test");
  ASSERT_TRUE(result.value().port.has_value());
  EXPECT_EQ(*result.value().port, 3000);
  ASSERT_TRUE(result.value().label.has_value());
  EXPECT_EQ(*result.value().label, "dev");
}

TEST(cli, test_optional_absent) {
  const char* args[] = {
      "program",
      "--name=test"
  };
  const auto result = rfl::cli::read<Config>(2, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().name, "test");
  EXPECT_FALSE(result.value().port.has_value());
  EXPECT_FALSE(result.value().label.has_value());
}

}  // namespace test_optional
