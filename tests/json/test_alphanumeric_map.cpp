#include <map>
#include <memory>
#include <string>

#include "write_and_read.hpp"
import rfl;
import rfl.json;


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
