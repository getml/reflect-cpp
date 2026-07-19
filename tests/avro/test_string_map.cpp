#include <map>
#include <memory>
#include <string>

#include "write_and_read.hpp"
import rfl.avro;
import rfl;
import rfl.json;


namespace test_string_map {
TEST(avro, test_string_map) {
  std::map<std::string, std::unique_ptr<std::string>> homer;
  homer.insert(
      std::make_pair("firstName", std::make_unique<std::string>("Homer")));
  homer.insert(
      std::make_pair("lastName", std::make_unique<std::string>("Simpson")));

  write_and_read_with_json(homer);
}
}  // namespace test_string_map
