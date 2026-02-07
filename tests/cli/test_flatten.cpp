#include <gtest/gtest.h>
#include <rfl.hpp>
#include <rfl/cli.hpp>
#include <string>

namespace test_flatten {

struct Address {
  std::string city;
  int zip;
};

struct Person {
  std::string name;
  rfl::Flatten<Address> address;
};

TEST(cli, test_flatten) {
  const char* args[] = {
      "program",
      "--name=Alice",
      "--city=Springfield",
      "--zip=12345"
  };
  const auto result = rfl::cli::read<Person>(4, const_cast<char**>(args));
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(result.value().name, "Alice");
  EXPECT_EQ(result.value().address().city, "Springfield");
  EXPECT_EQ(result.value().address().zip, 12345);
}

}  // namespace test_flatten
