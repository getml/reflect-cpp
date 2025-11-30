#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_default_val_no_field_names {

struct Person {
  std::string first_name;
  rfl::DefaultVal<std::string> last_name = "Simpson";
  rfl::DefaultVal<std::string> town;
};

TEST(json, test_default_val_no_field_names) {
  static_assert(rfl::internal::has_default_val_v<Person>,
                "Should have default val");

  const auto should_fail = rfl::json::read<Person, rfl::NoFieldNames>(R"([])");

  EXPECT_EQ(should_fail && true, false)
      << "Should have failed due to missing required field";

  const auto homer =
      rfl::json::read<Person, rfl::NoFieldNames>(R"(["Homer"])").value();

  EXPECT_EQ(Person{}.last_name.value(), "Simpson");
  EXPECT_EQ(homer.first_name, "Homer");
  EXPECT_EQ(homer.last_name.value(), "Simpson");
  EXPECT_EQ(homer.town.value(), "");

  write_and_read<rfl::NoFieldNames>(Person{"Waylon", "Smith", "Springfield"},
                                    R"(["Waylon","Smith","Springfield"])");
}
}  // namespace test_default_val_no_field_names
