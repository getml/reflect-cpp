#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "test_box.hpp"

namespace test_box2 {

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  auto ptr = std::make_unique<std::string>("Hello World!");
  const rfl::Result<rfl::Box<std::string>> box =
      rfl::make_box<std::string>(std::move(ptr));

  if (box) {
    std::cout << "OK" << std::endl << std::endl;
  } else {
    std::cout << "Expected the result to be successful." << std::endl
              << std::endl;
  }
}
}  // namespace test_box2
