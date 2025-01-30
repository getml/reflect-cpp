
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <tuple>

#include "write_and_read.hpp"

namespace test_rfl_tuple {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<rfl::Tuple<Person, Person, Person>> children = nullptr;
};

TEST(capnproto, test_rfl_tuple) {
  auto bart = Person{.first_name = "Bart"};

  auto lisa = Person{.first_name = "Lisa"};

  auto maggie = Person{.first_name = "Maggie"};

  const auto homer =
      Person{.first_name = "Homer",
             .children = std::make_unique<rfl::Tuple<Person, Person, Person>>(
                 rfl::Tuple<Person, Person, Person>{
                     std::move(bart), std::move(lisa), std::move(maggie)})};

  write_and_read(homer);
}
}  // namespace test_rfl_tuple
