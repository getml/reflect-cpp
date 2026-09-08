#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_camel_case_to_snake_case_rename {

struct Person {
  std::string firstName;
  std::string lastName;
  rfl::Rename<"homeTown", std::string> homeTown = "Springfield";
};

TEST(json, test_camel_case_to_snake_case_rename) {

  const auto homer = Person{.firstName = "Homer", .lastName = "Simpson"};

  write_and_read<rfl::CamelCaseToSnakeCase>(
      homer,
      R"({"first_name":"Homer","last_name":"Simpson","homeTown":"Springfield"})");
}

}  // namespace test_camel_case_to_snake_case_rename
