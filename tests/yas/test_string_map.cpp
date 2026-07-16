#include <map>
#include <memory>
#include <string>

#include "write_and_read.hpp"
import rfl.yas;
import rfl;
import rfl.json;


namespace test_string_map {

struct RootStruct {
  std::map<std::string, std::unique_ptr<std::string>> root;
};

TEST(yas, test_string_map) {
  RootStruct homer;
  homer.root.insert(
      std::make_pair("firstName", std::make_unique<std::string>("Homer")));
  homer.root.insert(
      std::make_pair("lastName", std::make_unique<std::string>("Simpson")));

  write_and_read(homer);
}
}  // namespace test_string_map
