#include <gtest/gtest.h>

#include <map>
#include <rfl.hpp>
#include <rfl/env.hpp>

#include "write_and_read.hpp"

namespace test_map_field {

struct Config {
  std::map<std::string, std::string> settings;
};

TEST(env, test_map_field) {
  auto settings = std::map<std::string, std::string>();
  settings["host"] = "localhost";
  settings["port"] = "8080";
  settings["debug"] = "true";

  const auto config = Config{.settings = std::move(settings)};

  write_and_read(config);
}

}  // namespace test_map_field
