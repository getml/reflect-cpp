#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_extra_fields_with_no_extra_fields {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  rfl::ExtraFields<rfl::Generic> extra_fields;
};

TEST(json, test_extra_fields_with_no_extra_fields) {
  auto homer = Person{.first_name = "Homer"};

  homer.extra_fields["age"] = 45;
  homer.extra_fields["email"] = "homer@simpson.com";
  homer.extra_fields["town"] = "Springfield";

  write_and_read<rfl::NoExtraFields>(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","age":45,"email":"homer@simpson.com","town":"Springfield"})");
}
}  // namespace test_extra_fields_with_no_extra_fields
