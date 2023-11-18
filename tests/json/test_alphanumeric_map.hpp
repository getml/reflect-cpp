#include <iostream>
#include <map>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

void test_alphanumeric_map() {
  std::cout << "test_alphanumeric_map" << std::endl;

  std::map<rfl::AlphaNumeric, std::unique_ptr<std::string>> homer;
  homer.insert(
      std::make_pair("firstName", std::make_unique<std::string>("Homer")));
  homer.insert(
      std::make_pair("lastName", std::make_unique<std::string>("Simpson")));

  write_and_read(homer, R"({"firstName":"Homer","lastName":"Simpson"})");
}
