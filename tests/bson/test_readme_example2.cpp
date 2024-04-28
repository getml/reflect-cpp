#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_readme_example2 {

struct Person {
  std::string first_name;
  std::string last_name;
  int age;
};

TEST(bson, test_readme_example2) {
  const auto homer =
      Person{.first_name = "Homer", .last_name = "Simpson", .age = 45};

  write_and_read(homer);
}
}  // namespace test_readme_example2
