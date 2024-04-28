#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_any_of {

using Age =
    rfl::Validator<unsigned int,
                   rfl::AnyOf<rfl::AllOf<rfl::Minimum<0>, rfl::Maximum<10>>,
                              rfl::AllOf<rfl::Minimum<40>, rfl::Maximum<130>>>>;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  Age age;
};

TEST(json, test_any_of) {
  const auto homer =
      Person{.first_name = "Homer", .last_name = "Simpson", .age = 45};

  write_and_read(homer,
                 R"({"firstName":"Homer","lastName":"Simpson","age":45})");
}

}  // namespace test_any_of
