#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_unordered_multiset {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::unordered_multiset<std::string>> children;
};

TEST(json, test_unordered_multiset) {
  auto children = std::make_unique<std::unordered_multiset<std::string>>(
      std::unordered_multiset<std::string>({"Bart", "Lisa", "Maggie"}));

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  // Unordered multisets are unpredictable. We therefore only make sure that
  // this compiles.
  const auto json_string = rfl::json::write(homer);
  const auto homer2 = rfl::json::read<Person>(json_string);
  EXPECT_TRUE(homer2 && true);
}
}  // namespace test_unordered_multiset
