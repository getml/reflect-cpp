#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_deque {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::deque<Person>> children;
};

TEST(xml, test_default_values) {
  auto children = std::make_unique<std::deque<Person>>();
  children->emplace_back(Person{.first_name = "Bart"});
  children->emplace_back(Person{.first_name = "Lisa"});
  children->emplace_back(Person{.first_name = "Maggie"});

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  write_and_read(homer);

}
}  // namespace test_deque
