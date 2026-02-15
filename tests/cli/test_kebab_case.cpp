#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/cli.hpp>
#include <string>

namespace test_kebab_case {

struct Config {
  std::string host_name;
  int max_retries;
  bool dry_run;
};

TEST(cli, test_kebab_case_conversion) {
  const char* args[] = {
      "program",
      "--host-name=server1",
      "--max-retries=3",
      "--dry-run"
  };
  const auto result = rfl::cli::read<Config>(4, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().host_name, "server1");
  EXPECT_EQ(result.value().max_retries, 3);
  EXPECT_TRUE(result.value().dry_run);
}

}  // namespace test_kebab_case
