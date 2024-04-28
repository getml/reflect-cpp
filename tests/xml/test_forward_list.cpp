#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_forward_list {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::forward_list<Person>> children;
};

TEST(xml, test_forward_list) {
  auto children = std::make_unique<std::forward_list<Person>>();
  children->emplace_front(Person{.first_name = "Maggie"});
  children->emplace_front(Person{.first_name = "Lisa"});
  children->emplace_front(Person{.first_name = "Bart"});

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  write_and_read(homer);

}
}  // namespace test_forward_list
