
#include <memory>
#include <string>
#include <tuple>

#include "write_and_read.hpp"
#include <utility>

import rfl.avro;
import rfl;
import rfl.json;


namespace test_tuple {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::tuple<Person, Person, Person>> children = nullptr;
};

TEST(avro, test_tuple) {
  auto bart = Person{.first_name = "Bart"};

  auto lisa = Person{.first_name = "Lisa"};

  auto maggie = Person{.first_name = "Maggie"};

  const auto homer =
      Person{.first_name = "Homer",
             .children = std::make_unique<std::tuple<Person, Person, Person>>(
                 std::tuple<Person, Person, Person>{
                     std::move(bart), std::move(lisa), std::move(maggie)})};

  write_and_read_with_json(homer);
}
}  // namespace test_tuple
