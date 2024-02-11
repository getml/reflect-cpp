#include "test_wstring.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>

#include "write_and_read.hpp"

namespace test_wstring {
void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  std::map<std::string, std::wstring> homer;
  homer.insert(std::make_pair("firstName", L"Homer"));

  write_and_read(homer, R"({"firstName":"Homer"})");
}
}  // namespace test_wstring
