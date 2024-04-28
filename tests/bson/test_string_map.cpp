#include <iostream>
#include <map>
#include <memory>
#include <rfl.hpp>
#include <source_location>
#include <string>

#include "write_and_read.hpp"

namespace test_string_map {
TEST(bson, test_string_map) { 
  std::map<std::string, std::unique_ptr<std::string>> homer;
  homer.insert(
      std::make_pair("firstName", std::make_unique<std::string>("Homer")));
  homer.insert(
      std::make_pair("lastName", std::make_unique<std::string>("Simpson")));

  write_and_read(homer);
}
}  // namespace test_string_map
