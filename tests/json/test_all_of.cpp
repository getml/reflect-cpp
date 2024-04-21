#include <iostream>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

// Make sure things still compile when
// rfl.hpp is included after rfl/json.hpp.
#include <rfl.hpp>

#include "write_and_read.hpp"

namespace test_all_of {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  Age age;
};

TEST(json, test_all_of) {
  const auto homer =
      Person{.first_name = "Homer", .last_name = "Simpson", .age = 45};

  write_and_read(homer,
                 R"({"firstName":"Homer","lastName":"Simpson","age":45})");
}
}  // namespace test_all_of
