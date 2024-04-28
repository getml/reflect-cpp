#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_result {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::vector<rfl::Result<Person>> children;
};

TEST(json, test_result) {
  const rfl::Result<Person> bart = Person{.first_name = "Bart"};

  const rfl::Result<Person> lisa = Person{.first_name = "Lisa"};

  const rfl::Result<Person> maggie = rfl::Error("Some error occurred.");

  const auto homer = Person{
      .first_name = "Homer",
      .children = std::vector<rfl::Result<Person>>({bart, lisa, maggie})};

  write_and_read(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","children":[{"firstName":"Bart","lastName":"Simpson","children":[]},{"firstName":"Lisa","lastName":"Simpson","children":[]},{"error":"Some error occurred."}]})");
}
}  // namespace test_result
