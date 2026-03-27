#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_readme_example {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::string town = "Springfield";
  int age;
};

TEST(boost_serialization, test_readme_example) {
  const auto homer =
      Person{.first_name = "Homer", .last_name = "Simpson", .age = 45};

  write_and_read_with_json(homer);
}
}  // namespace test_readme_example
