#include <gtest/gtest.h>

#include <rfl.hpp>
#include <rfl/env.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_extra_fields {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  rfl::ExtraFields<rfl::Generic> extra_fields;
};

TEST(env, test_extra_fields) {
  const auto scoped_environment = test_env::ScopedEnvironment();

  auto homer = Person{.first_name = "Homer"};

  homer.extra_fields["age"] = 45;
  homer.extra_fields["email"] = "homer@simpson.com";
  homer.extra_fields["town"] = "Springfield";

  rfl::env::write(homer);

  const auto read_homer = rfl::env::read<Person>();

  ASSERT_TRUE(read_homer);
  ASSERT_EQ(read_homer->first_name(), "Homer");
  ASSERT_EQ(read_homer->last_name(), "Simpson");
  ASSERT_EQ(read_homer->extra_fields.size(), 0);
}
}  // namespace test_extra_fields
