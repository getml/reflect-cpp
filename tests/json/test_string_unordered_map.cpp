#include <iostream>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <unordered_map>

#include "write_and_read.hpp"

namespace test_string_unordered_map {
TEST(json, test_string_unordered_map) {
  using Map = std::unordered_map<std::string, std::unique_ptr<std::string>>;
  Map homer;
  homer.insert(
      std::make_pair("firstName", std::make_unique<std::string>("Homer")));
  homer.insert(
      std::make_pair("lastName", std::make_unique<std::string>("Simpson")));

  // Unordered maps are unpredictable. We therefore only make sure that this
  // compiles.
  const auto json_string = rfl::json::write(homer);
  const auto homer2 = rfl::json::read<Map>(json_string);
  EXPECT_TRUE(true);
}
}  // namespace test_string_unordered_map
