#include <iostream>
#include <map>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_string_map {

struct RootStruct {
  std::map<std::string, std::unique_ptr<std::string>> root;
};

TEST(capnproto, test_string_map) {
  RootStruct homer;
  homer.root.insert(
      std::make_pair("firstName", std::make_unique<std::string>("Homer")));
  homer.root.insert(
      std::make_pair("lastName", std::make_unique<std::string>("Simpson")));

  write_and_read(homer);
}
}  // namespace test_string_map
