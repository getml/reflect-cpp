#include <gtest/gtest.h>

#include <map>
#include <rfl.hpp>
#include <rfl/env.hpp>
#include <string>

namespace test_map {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::map<std::string, Person> children;
};

TEST(env, test_map) {
  auto children = std::map<std::string, Person>();
  children.insert(std::make_pair("child1", Person{.first_name = "Bart"}));
  children.insert(std::make_pair("child2", Person{.first_name = "Lisa"}));
  children.insert(std::make_pair("child3", Person{.first_name = "Maggie"}));

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};
}
}  // namespace test_map
