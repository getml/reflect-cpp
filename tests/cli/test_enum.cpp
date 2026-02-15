#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/cli.hpp>
#include <string>

namespace test_enum {

enum class Mode { debug, release, test };

struct Config {
  std::string name;
  Mode mode;
};

TEST(cli, test_enum) {
  const char* args[] = {
      "program",
      "--name=app",
      "--mode=release"
  };
  const auto result = rfl::cli::read<Config>(3, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().name, "app");
  EXPECT_EQ(result.value().mode, Mode::release);
}

}  // namespace test_enum
