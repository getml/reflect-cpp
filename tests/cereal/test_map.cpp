#include <iostream>
#include <map>
#include <rfl.hpp>
#include <rfl/cereal.hpp>
#include <string>
#include <unordered_map>

#include "write_and_read.hpp"

namespace test_map {
struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::map<std::string, Person>> children;
};

TEST(cereal, test_map) {
  auto bart = Person{.first_name = "Bart"};

  auto lisa = Person{.first_name = "Lisa"};

  auto maggie = Person{.first_name = "Maggie"};

  auto children = std::make_unique<std::map<std::string, Person>>(
      std::map<std::string, Person>({{"Bart", std::move(bart)},
                                     {"Lisa", std::move(lisa)},
                                     {"Maggie", std::move(maggie)}}));

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  write_and_read(homer);
}
}  // namespace test_map
