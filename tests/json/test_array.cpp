
#include <array>
#include <iostream>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>

#include "write_and_read.hpp"

namespace test_array {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::array<Person, 3>> children = nullptr;
};

TEST(json, test_array) {
  auto bart = Person{.first_name = "Bart"};

  auto lisa = Person{.first_name = "Lisa"};

  auto maggie = Person{.first_name = "Maggie"};

  const auto homer = Person{
      .first_name = "Homer",
      .children = std::make_unique<std::array<Person, 3>>(std::array<Person, 3>{
          std::move(bart), std::move(lisa), std::move(maggie)})};

  write_and_read(
      homer,
      "{\"firstName\":\"Homer\",\"lastName\":\"Simpson\",\"children\":[{"
      "\"firstName\":\"Bart\",\"lastName\":\"Simpson\"},{"
      "\"firstName\":\"Lisa\",\"lastName\":\"Simpson\"},{"
      "\"firstName\":\"Maggie\",\"lastName\":\"Simpson\"}]}");
}
}  // namespace test_array
