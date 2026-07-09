#include <gtest/gtest.h>

#include <rfl.hpp>
#include <rfl/env.hpp>
#include <string>

namespace test_basic {

struct Settings {
  std::string host;
  int port;
  double rate;
  bool verbose;
};

TEST(env, test_basic) {
  const auto settings = Settings{
      .host = "localhost",
      .port = 8080,
      .rate = 1.5,
      .verbose = true,
  };

  rfl::env::write(settings);

  ASSERT_EQ(std::getenv("HOST"), std::string("localhost"));
  ASSERT_EQ(std::getenv("PORT"), std::string("8080"));
  ASSERT_EQ(std::getenv("RATE"), std::string("1.500000"));
  ASSERT_EQ(std::getenv("VERBOSE"), std::string("true"));

  const auto read_settings = rfl::env::read<Settings>();

  ASSERT_TRUE(read_settings);
  ASSERT_EQ(read_settings->host, "localhost");
  ASSERT_EQ(read_settings->port, 8080);
  ASSERT_DOUBLE_EQ(read_settings->rate, 1.5);
  ASSERT_EQ(read_settings->verbose, true);
}

}  // namespace test_basic
