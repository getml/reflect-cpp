#include <string>

#include "write_and_read.hpp"
#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


namespace test_skip {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  rfl::Skip<std::string> town;
  Age age;
};

TEST(json, test_skip) {
  const auto homer = Person{.first_name = "Homer",
                            .last_name = "Simpson",
                            .town = "Springfield",
                            .age = 45};

  write_and_read(homer,
                 R"({"firstName":"Homer","lastName":"Simpson","age":45})");
}
}  // namespace test_skip
