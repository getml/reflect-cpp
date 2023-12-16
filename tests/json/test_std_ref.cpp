#include "test_std_ref.hpp"

#include <functional>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>

namespace test_std_ref {

struct StdRefStruct {
  std::reference_wrapper<int> a;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  int i = 10;
  StdRefStruct struct_ = {.a = std::ref(i)};

  const auto json_string = rfl::json::write(struct_);
  const std::string& expected = R"({"a":10})";
  if (json_string != expected) {
    std::cout << "Test failed on write. Expected:" << std::endl
              << expected << std::endl
              << "Got: " << std::endl
              << json_string << std::endl
              << std::endl;
    return;
  }
  std::cout << "OK" << std::endl << std::endl;
}

}  // namespace test_std_ref
