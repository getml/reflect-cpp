#include <iostream>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <unordered_map>

#include "write_and_read.hpp"

void test_alphanumeric_unordered_map() {
  std::cout << "test_alphanumeric_unordered_map" << std::endl;

  using Map =
      std::unordered_map<rfl::AlphaNumeric, std::unique_ptr<std::string>>;
  Map homer;
  homer.insert(
      std::make_pair("firstName", std::make_unique<std::string>("Homer")));
  homer.insert(
      std::make_pair("lastName", std::make_unique<std::string>("Simpson")));

  // Unordered maps are unpredictable. We therefore only make sure that this
  // compiles.
  const auto json_string = rfl::json::write(homer);
  const auto homer2 = rfl::json::read<Map>(json_string);
  std::cout << "OK" << std::endl << std::endl;
}
