#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_comment {

struct Person {
  std::string first_name;
  std::string last_name;
  rfl::Commented<std::string> town;
};

TEST(xml, test_comment) {
  const auto homer = Person{.first_name = "Homer",
                            .last_name = "Simpson",
                            .town = rfl::Commented<std::string>(
                                "Springfield", "The town where Homer lives")};

  const auto xml_str = rfl::xml::write(homer);

  EXPECT_EQ(xml_str, R"(<?xml version="1.0" encoding="UTF-8"?>
<Person>
    <first_name>Homer</first_name>
    <last_name>Simpson</last_name>
    <town>Springfield</town>
    <!--The town where Homer lives-->
</Person>
)");
}
}  // namespace test_comment
