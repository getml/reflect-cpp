#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_snake_case_to_camel_case_rename {

struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Rename<"home_town", std::string> home_town = "Springfield";
};

TEST(json, test_snake_case_to_camel_case_rename) {
  const auto homer = Person{.first_name = "Homer", .last_name = "Simpson"};

  write_and_read<rfl::SnakeCaseToCamelCase>(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","home_town":"Springfield"})");
}
}  // namespace test_snake_case_to_camel_case_rename
