#include <iostream>
#include <map>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>

#include "write_and_read.hpp"

namespace test_map_with_key_validation {

using Between1and3 = rfl::Validator<int, rfl::Minimum<1>, rfl::Maximum<3>>;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::map<Between1and3, Person>> children;
};

TEST(json, test_map_with_key_validation) {
  auto children = std::make_unique<std::map<Between1and3, Person>>();

  children->insert(std::make_pair(1, Person{.first_name = "Bart"}));
  children->insert(std::make_pair(2, Person{.first_name = "Lisa"}));
  children->insert(std::make_pair(3, Person{.first_name = "Maggie"}));

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  write_and_read(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","children":{"1":{"firstName":"Bart","lastName":"Simpson"},"2":{"firstName":"Lisa","lastName":"Simpson"},"3":{"firstName":"Maggie","lastName":"Simpson"}}})");
}
}  // namespace test_map_with_key_validation
