#include <gtest/gtest.h>

#include <rfl.hpp>
#include <rfl/env.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_nested {

struct NestedSettings {
  std::string database_host;
  int database_port;
};

struct Settings {
  std::string host;
  int port;
  double rate;
  bool verbose;
  NestedSettings nested;
};

TEST(env, test_nested) {
  const auto settings = Settings{.host = "localhost",
                                 .port = 8080,
                                 .rate = 1.5,
                                 .verbose = true,
                                 .nested = NestedSettings{
                                     .database_host = "localhost",
                                     .database_port = 8080,
                                 }};

  write_and_read(settings, []() {
    ASSERT_EQ(std::getenv("HOST"), std::string("localhost"));
    ASSERT_EQ(std::getenv("PORT"), std::string("8080"));
    ASSERT_EQ(std::getenv("RATE"), std::string("1.500000"));
    ASSERT_EQ(std::getenv("VERBOSE"), std::string("true"));
    ASSERT_EQ(std::getenv("NESTED_DATABASE_HOST"), std::string("localhost"));
    ASSERT_EQ(std::getenv("NESTED_DATABASE_PORT"), std::string("8080"));
  });
}

}  // namespace test_nested
