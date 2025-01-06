#include <iostream>
#include <map>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_map2 {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::map<std::string, Person> children;
};

TEST(capnproto, test_map2) {
  auto children = std::map<std::string, Person>();
  children.insert(std::make_pair("Bart", Person{.first_name = "Bart"}));
  children.insert(std::make_pair("Lisa", Person{.first_name = "Lisa"}));
  children.insert(std::make_pair("Maggie", Person{.first_name = "Maggie"}));

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  write_and_read(homer);
}
}  // namespace test_map2
