#include <iostream>
#include <map>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>

#include "write_and_read.hpp"

namespace test_wstring {
TEST(json, test_wstring) {
  std::map<std::string, std::wstring> homer;
  homer.insert(std::make_pair("firstName", L"Homer"));

  write_and_read(homer, R"({"firstName":"Homer"})");
}
}  // namespace test_wstring
