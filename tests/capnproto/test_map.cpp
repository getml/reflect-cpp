#include <map>
#include <string>

#include "write_and_read.hpp"
#include <memory>
#include <utility>

import rfl.capnproto;
import rfl;
import rfl.json;


namespace test_map {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::map<std::string, Person>> children;
};

TEST(capnproto, test_map) {
  auto children = std::make_unique<std::map<std::string, Person>>();
  children->insert(std::make_pair("Bart", Person{.first_name = "Bart"}));
  children->insert(std::make_pair("Lisa", Person{.first_name = "Lisa"}));
  children->insert(std::make_pair("Maggie", Person{.first_name = "Maggie"}));

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  write_and_read(homer);
}
}  // namespace test_map
