
#include <gtest/gtest.h>
#include <string>
#include <vector>

import rfl;


namespace test_meta_fields {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::string town = "Springfield";
  unsigned int age;
  std::vector<Person> children;
};

TEST(json, test_meta_fields) {
  const auto fields = rfl::fields<Person>();

  EXPECT_EQ(fields.size(), 5);
}
}  // namespace test_meta_fields
