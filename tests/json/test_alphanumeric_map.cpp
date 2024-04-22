#include <iostream>
#include <map>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>

#include "write_and_read.hpp"

namespace test_alphanumeric_map {

TEST(json, test_alphanumeric_map) {
  std::map<rfl::AlphaNumeric, std::unique_ptr<std::string>> homer;
  homer.insert(
      std::make_pair("firstName", std::make_unique<std::string>("Homer")));
  homer.insert(
      std::make_pair("lastName", std::make_unique<std::string>("Simpson")));

  write_and_read(homer, R"({"firstName":"Homer","lastName":"Simpson"})");
}

}  // namespace test_alphanumeric_map
