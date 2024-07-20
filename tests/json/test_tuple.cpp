
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <tuple>

#include "write_and_read.hpp"

namespace test_tuple {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::tuple<Person, Person, Person>> children = nullptr;
};

TEST(json, test_tuple) {
  auto bart = Person{.first_name = "Bart"};

  auto lisa = Person{.first_name = "Lisa"};

  auto maggie = Person{.first_name = "Maggie"};

  const auto homer =
      Person{.first_name = "Homer",
             .children = std::make_unique<std::tuple<Person, Person, Person>>(
                 std::tuple<Person, Person, Person>{
                     std::move(bart), std::move(lisa), std::move(maggie)})};

  write_and_read(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","children":[{"firstName":"Bart","lastName":"Simpson"},{"firstName":"Lisa","lastName":"Simpson"},{"firstName":"Maggie","lastName":"Simpson"}]})");
}
}  // namespace test_tuple
