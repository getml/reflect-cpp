#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <unordered_map>

#include "write_and_read.hpp"

namespace test_unordered_map_with_custom_hash {

template <typename T>
struct DummyHash {
  std::size_t operator()(const T& v) const { return std::hash<T>()(v); }
};

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::unordered_map<int, Person, DummyHash<int>>> children;
};

TEST(json, test_unordered_map_with_custom_hash) {
  auto children =
      std::make_unique<std::unordered_map<int, Person, DummyHash<int>>>();
  children->insert(std::make_pair(1, Person{.first_name = "Bart"}));
  children->insert(std::make_pair(2, Person{.first_name = "Lisa"}));
  children->insert(std::make_pair(3, Person{.first_name = "Maggie"}));

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  // Unordered maps are unpredictable. We therefore only make sure that this
  // compiles.
  const auto json_string = rfl::json::write(homer);
  const auto homer2 = rfl::json::read<Person>(json_string);

  EXPECT_TRUE(true);
}
}  // namespace test_unordered_map_with_custom_hash
