#include <map>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_map_with_custom_less {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::shared_ptr<std::map<size_t, Person, std::less<>>> children;
};

TEST(json, test_map_with_custom_less) {
  auto children = std::make_shared<std::map<size_t, Person, std::less<>>>();
  children->insert(std::make_pair(1, Person{.first_name = "Bart"}));
  children->insert(std::make_pair(2, Person{.first_name = "Lisa"}));
  children->insert(std::make_pair(2660778562, Person{.first_name = "Maggie"}));

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  write_and_read(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","children":{"1":{"firstName":"Bart","lastName":"Simpson"},"2":{"firstName":"Lisa","lastName":"Simpson"},"2660778562":{"firstName":"Maggie","lastName":"Simpson"}}})");
}
}  // namespace test_map_with_custom_less
