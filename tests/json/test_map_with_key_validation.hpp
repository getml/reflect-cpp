#include <iostream>
#include <map>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

void test_map_with_key_validation() {
  std::cout << "test_map_with_key_validation" << std::endl;

  using Between1and3 = rfl::Validator<int, rfl::Minimum<1>, rfl::Maximum<3>>;

  struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name = "Simpson";
    rfl::Field<"children", std::unique_ptr<std::map<Between1and3, Person>>>
        children = rfl::default_value;
  };

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
