#include <gtest/gtest.h>

#include <rfl.hpp>
#include <rfl/env.hpp>
#include <string>

#include "write_and_read.hpp"

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

  write_and_read(settings, []() {
    ASSERT_EQ(std::getenv("HOST"), std::string("localhost"));
    ASSERT_EQ(std::getenv("PORT"), std::string("8080"));
    ASSERT_EQ(std::getenv("RATE"), std::string("1.500000"));
    ASSERT_EQ(std::getenv("VERBOSE"), std::string("true"));
  });
}

}  // namespace test_basic
