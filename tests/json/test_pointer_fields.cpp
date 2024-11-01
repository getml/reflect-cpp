#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_pointer_fields {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::vector<Person>* children;
};

TEST(json, test_pointer_fields) {
  const auto bart = Person{.first_name = "Bart", .children = nullptr};

  const auto lisa = Person{.first_name = "Lisa", .children = nullptr};

  const auto maggie = Person{.first_name = "Maggie", .children = nullptr};

  const auto homer =
      Person{.first_name = "Homer",
             .children = new std::vector<Person>({bart, lisa, maggie})};

  const std::string expected =
      R"({"firstName":"Homer","lastName":"Simpson","children":[{"firstName":"Bart","lastName":"Simpson","children":null},{"firstName":"Lisa","lastName":"Simpson","children":null},{"firstName":"Maggie","lastName":"Simpson","children":null}]})";

  const auto json_str = rfl::json::write(homer);

  EXPECT_EQ(json_str, expected);

  const auto homer2 =
      rfl::json::read<Person, rfl::AllowRawPtrs>(json_str).value();
  const auto json_str2 = rfl::json::write(homer2);

  EXPECT_EQ(json_str2, expected);

  delete homer.children;
  delete homer2.children;
}
}  // namespace test_pointer_fields
