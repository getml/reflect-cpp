#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_default_val {

struct Person {
  std::string first_name;
  rfl::DefaultVal<std::string> last_name = "Simpson";
  rfl::DefaultVal<std::string> town;
  rfl::Rename<"country", rfl::DefaultVal<std::string>> home_country = "USA";
};

TEST(json, test_default_val) {
  static_assert(rfl::internal::has_default_val_v<Person>,
                "Should have default val");

  const auto should_fail = rfl::json::read<Person>(R"({})");

  EXPECT_EQ(should_fail && true, false)
      << "Should have failed due to missing required field";

  const auto homer =
      rfl::json::read<Person>(R"({"first_name":"Homer"})").value();

  EXPECT_EQ(Person{}.last_name.value(), "Simpson");
  EXPECT_EQ(homer.first_name, "Homer");
  EXPECT_EQ(homer.last_name.value(), "Simpson");
  EXPECT_EQ(homer.town.value(), "");
  EXPECT_EQ(homer.home_country().value(), "USA");

  write_and_read(
      Person{"Waylon", "Smith", "Springfield"},
      R"({"first_name":"Waylon","last_name":"Smith","town":"Springfield","country":"USA"})");
}
}  // namespace test_default_val
