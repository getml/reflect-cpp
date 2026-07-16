#include <memory>
#include <string>
#include <vector>

#include "write_and_read.hpp"
#include <utility>

import rfl.capnproto;
import rfl;
import rfl.json;


namespace test_shared_ptr {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::shared_ptr<std::vector<Person>> children;
};

TEST(capnproto, test_shared_ptr) {
  auto children = std::make_shared<std::vector<Person>>();
  children->emplace_back(Person{.first_name = "Bart"});
  children->emplace_back(Person{.first_name = "Lisa"});
  children->emplace_back(Person{.first_name = "Maggie"});

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  write_and_read(homer);
}
}  // namespace test_shared_ptr
