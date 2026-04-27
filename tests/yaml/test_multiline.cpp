#include <rfl.hpp>
#include <string>

#include "write_and_read.hpp"

struct MultilineTestStruct {
  std::string normal_string;
  std::string multiline_string;
};

namespace test_multiline {
TEST(yaml, test_multiline) {
  const auto test = MultilineTestStruct{.normal_string = "The normal string",
                               .multiline_string =
R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod
tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,
quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo
consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum
dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident,
sunt in culpa qui officia deserunt mollit anim id est laborum.)"
  };

  write_and_read(test, rfl::yaml::Writer::string_multiline_literal);
  write_and_read(test, rfl::yaml::Writer::string_all_literal);
}

TEST(yaml, test_multiline_read) {
  const auto test = MultilineTestStruct{.normal_string = "The normal string",
                               .multiline_string =
R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod
tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, etc...)"
  };

  const std::string random_yaml(
R"(
normal_string: |
  The normal string


multiline_string: |
  Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod
  tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, etc...

)"
  );

  auto read_result = rfl::yaml::read<MultilineTestStruct>(random_yaml);
  EXPECT_TRUE(read_result.has_value());
  EXPECT_EQ(read_result.value().normal_string, test.normal_string);
  EXPECT_EQ(read_result.value().multiline_string, test.multiline_string);
}
}  // namespace test_multiline
