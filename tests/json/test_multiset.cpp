#include <string>

#include "write_and_read.hpp"
#include <memory>
#include <set>
#include <utility>

import rfl;
import rfl.json;


namespace test_multiset {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::multiset<std::string>> children;
};

TEST(json, test_multiset) {
  auto children = std::make_unique<std::multiset<std::string>>(
      std::multiset<std::string>({"Bart", "Lisa", "Maggie"}));

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  write_and_read(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","children":["Bart","Lisa","Maggie"]})");
}
}  // namespace test_multiset
