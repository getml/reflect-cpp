#include <gtest/gtest.h>

#include <rfl.hpp>
#include <rfl/env.hpp>
#include <string>

namespace test_skip {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  rfl::Skip<std::string> town;
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  Age age;
};

TEST(env, test_skip) {
  const auto homer = Person{.town = "Springfield",
                            .first_name = "Homer",
                            .last_name = "Simpson",
                            .age = 45};
}
}  // namespace test_skip
