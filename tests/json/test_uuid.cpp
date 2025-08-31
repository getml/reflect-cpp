#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_uuid {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  rfl::UUIDv4 uuid;
};

TEST(json, test_uuid) {
  const auto homer = Person{.first_name = "Homer",
                            .last_name = "Simpson",
                            .uuid = "f23b2a1d-b1a6-4e0d-af1b-8f0d912e255a"};

  write_and_read(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","uuid":"f23b2a1d-b1a6-4e0d-af1b-8f0d912e255a"})");
}
}  // namespace test_uuid
