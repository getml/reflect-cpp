#include <gtest/gtest.h>

#include <rfl.hpp>
#include <rfl/env.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_vector {

struct Settings {
  std::string host;
  int port;
  double rate;
  bool verbose;
  std::vector<std::string> tags;
};

TEST(env, test_vector) {
  const auto settings = Settings{.host = "localhost",
                                 .port = 8080,
                                 .rate = 1.5,
                                 .verbose = true,
                                 .tags = {"tag1", "tag2", "tag3"}};

  write_and_read(settings, []() {
    ASSERT_TRUE(std::getenv("HOST"));
    ASSERT_EQ(std::getenv("HOST"), std::string("localhost"));
    ASSERT_TRUE(std::getenv("PORT"));
    ASSERT_EQ(std::getenv("PORT"), std::string("8080"));
    ASSERT_TRUE(std::getenv("RATE"));
    ASSERT_EQ(std::getenv("RATE"), std::string("1.500000"));
    ASSERT_TRUE(std::getenv("VERBOSE"));
    ASSERT_EQ(std::getenv("VERBOSE"), std::string("true"));
    ASSERT_TRUE(std::getenv("TAGS_0"));
    ASSERT_EQ(std::getenv("TAGS_0"), std::string("tag1"));
    ASSERT_EQ(std::getenv("TAGS_0"), std::string("tag1"));
    ASSERT_EQ(std::getenv("TAGS_1"), std::string("tag2"));
    ASSERT_EQ(std::getenv("TAGS_2"), std::string("tag3"));
  });
}

}  // namespace test_vector
