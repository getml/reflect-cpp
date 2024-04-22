#include <iostream>
#include <map>
#include <rfl.hpp>
#include <source_location>
#include <string>

#include "write_and_read.hpp"

namespace test_map_with_key_validation {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::map<rfl::AlphaNumeric, Person>> children;
};

TEST(xml, test_map_with_key_validation) {
  auto children = std::make_unique<std::map<rfl::AlphaNumeric, Person>>();

  children->insert(std::make_pair("Bart", Person{.first_name = "Bart"}));
  children->insert(std::make_pair("Lisa", Person{.first_name = "Lisa"}));
  children->insert(std::make_pair("Maggie", Person{.first_name = "Maggie"}));

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  write_and_read(homer);
}
}  // namespace test_map_with_key_validation
