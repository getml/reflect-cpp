#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/cli.hpp>
#include <string>

namespace test_nested {

struct Database {
  std::string host;
  int port;
};

struct Server {
  std::string name;
  Database database;
};

TEST(cli, test_nested) {
  const char* args[] = {
      "program",
      "--name=myserver",
      "--database.host=db.local",
      "--database.port=5432"
  };
  const auto result = rfl::cli::read<Server>(4, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().name, "myserver");
  EXPECT_EQ(result.value().database.host, "db.local");
  EXPECT_EQ(result.value().database.port, 5432);
}

}  // namespace test_nested
