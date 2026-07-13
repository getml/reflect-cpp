#include <gtest/gtest.h>

#include <memory>
#include <rfl.hpp>
#include <rfl/env.hpp>
#include <string>
#include <vector>

namespace test_unique_ptr {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::vector<Person>> children;
};

TEST(env, test_unique_ptr) {
  auto children = std::make_unique<std::vector<Person>>();
  children->emplace_back(Person{.first_name = "Bart"});
  children->emplace_back(Person{.first_name = "Lisa"});
  children->emplace_back(Person{.first_name = "Maggie"});

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};
}
}  // namespace test_unique_ptr
