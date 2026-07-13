#include <gtest/gtest.h>

#include <rfl.hpp>
#include <rfl/env.hpp>

#include "write_and_read.hpp"

namespace test_string_map {

TEST(env, test_string_map) {
  std::map<std::string, std::unique_ptr<std::string>> homer;
  homer.insert(
      std::make_pair("firstName", std::make_unique<std::string>("Homer")));
  homer.insert(
      std::make_pair("lastName", std::make_unique<std::string>("Simpson")));

  write_and_read(homer);
}
}  // namespace test_string_map
