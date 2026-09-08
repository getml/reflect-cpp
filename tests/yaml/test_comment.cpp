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

TEST(yaml, test_comment) {
  const auto homer = Person{.first_name = "Homer",
                            .last_name = "Simpson",
                            .town = rfl::Commented<std::string>(
                                "Springfield", "The town where Homer lives")};

  const auto yaml_str = rfl::yaml::write(homer);

  EXPECT_EQ(yaml_str, R"(first_name: Homer
last_name: Simpson
town: Springfield  # The town where Homer lives)");
}
}  // namespace test_comment
