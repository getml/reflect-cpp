#include <gtest/gtest.h>

#include <rfl.hpp>
#include <rfl/env.hpp>
#include <string>

namespace test_timestamp {

using TS = rfl::Timestamp<"%Y-%m-%d">;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  TS birthday;
};

TEST(env, test_timestamp) {
  const auto result = TS::from_string("nonsense");

  if (result) {
    std::cout << "Failed: Expected an error, but got none." << std::endl;
    return;
  }

  const auto bart = Person{.first_name = "Bart", .birthday = "1987-04-19"};
}
}  // namespace test_timestamp
