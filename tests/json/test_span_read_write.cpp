#include <iostream>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <span>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_span {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::span<Person> children;
};

TEST(json, test_span) {
  auto children = std::vector<Person>({Person{.first_name = "Bart"},
                                       Person{.first_name = "Lisa"},
                                       Person{.first_name = "Maggie"}});

  const auto homer =
      Person{.first_name = "Homer",
             .children = std::span<Person>(children.data() + 1,
                                           children.data() + children.size())};

  const auto json_string = rfl::json::write(homer);

  const std::string expected =
      R"({"firstName":"Homer","lastName":"Simpson","children":[{"firstName":"Lisa","lastName":"Simpson","children":[]},{"firstName":"Maggie","lastName":"Simpson","children":[]}]})";

  const auto json_string1 = rfl::json::write(homer);
  EXPECT_EQ(json_string1, expected)
      << "Test failed on write. Expected:" << std::endl
      << expected << std::endl
      << "Got: " << std::endl
      << json_string1 << std::endl
      << std::endl;
  const auto res = rfl::json::read<Person, rfl::AllowRawPtrs>(json_string1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  const auto json_string2 = rfl::json::write(res.value());
  EXPECT_EQ(json_string2, expected)
      << "Test failed on read. Expected:" << std::endl
      << expected << std::endl
      << "Got: " << std::endl
      << json_string2 << std::endl
      << std::endl;

  if (res) {
    delete[] res->children.data();
  }
}
}  // namespace test_span
