#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

void test_result() {
  std::cout << "test_result" << std::endl;

  struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name = "Simpson";
    rfl::Field<"children", std::vector<rfl::Result<Person>>> children =
        rfl::default_value;
  };

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
