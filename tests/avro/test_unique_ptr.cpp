#include <memory>
#include <string>
#include <vector>

#include "write_and_read.hpp"
#include <utility>

import rfl.avro;
import rfl;
import rfl.json;


namespace test_unique_ptr {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::unique_ptr<std::vector<Person>> children;
};

TEST(avro, test_unique_ptr) {
  auto children = std::make_unique<std::vector<Person>>();
  children->emplace_back(Person{.first_name = "Bart"});
  children->emplace_back(Person{.first_name = "Lisa"});
  children->emplace_back(Person{.first_name = "Maggie"});

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  write_and_read(homer);
}
}  // namespace test_unique_ptr
