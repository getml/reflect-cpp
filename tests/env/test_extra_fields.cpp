#include <gtest/gtest.h>

#include <rfl.hpp>
#include <rfl/env.hpp>
#include <string>

namespace test_extra_fields {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  rfl::ExtraFields<rfl::Generic> extra_fields;
};

TEST(env, test_extra_fields) {
  auto homer = Person{.first_name = "Homer"};

  homer.extra_fields["age"] = 45;
  homer.extra_fields["email"] = "homer@simpson.com";
  homer.extra_fields["town"] = "Springfield";
}
}  // namespace test_extra_fields
