#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "test_readme_example.hpp"
#include "write_and_read.hpp"

namespace test_readme_example2 {

struct Person {
  std::string first_name;
  std::string last_name;
  int age;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const auto homer =
      Person{.first_name = "Homer", .last_name = "Simpson", .age = 45};

  write_and_read(homer);
}
}  // namespace test_readme_example2
