#include <gtest/gtest.h>

#include <rfl.hpp>
#include <rfl/env.hpp>
#include <string>

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

  rfl::env::write(settings);

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

  const auto read_settings = rfl::env::read<Settings>();

  ASSERT_TRUE(read_settings);
  ASSERT_EQ(read_settings->host, "localhost");
  ASSERT_EQ(read_settings->port, 8080);
  ASSERT_DOUBLE_EQ(read_settings->rate, 1.5);
  ASSERT_EQ(read_settings->verbose, true);
  ASSERT_EQ(read_settings->tags.size(), 3);
  ASSERT_EQ(read_settings->tags[0], "tag1");
  ASSERT_EQ(read_settings->tags[1], "tag2");
  ASSERT_EQ(read_settings->tags[2], "tag3");
}

}  // namespace test_vector
