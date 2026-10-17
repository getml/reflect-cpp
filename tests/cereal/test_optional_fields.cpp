#include <iostream>
#include <string>

#include <optional>

#include "write_and_read.hpp"

#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.cereal.hpp>


namespace test_optional_fields {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  std::optional<std::string> town = std::nullopt;
};

TEST(cereal, test_optional_fields) {
  const auto person1 = Person{.first_name = "Homer", .last_name = "Simpson"};
  write_and_read(person1);

  const auto person2 = Person{
      .first_name = "Homer", .last_name = "Simpson", .town = "Springfield"};
  write_and_read(person2);
}
}  // namespace test_optional_fields
