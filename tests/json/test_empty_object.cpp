#include "test_empty_object.hpp"

#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_empty_object {

struct Empty {};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const auto empty = Empty{};

  write_and_read(empty, R"({})");
}
}  // namespace test_empty_object
