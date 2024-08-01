#include <filesystem>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_filepath {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::filesystem::path path;
};

TEST(json, test_filepath) {
  auto homer =
      Person{.first_name = "Homer", .path = "/usr/lib/homer_simpson.cf"};

  write_and_read(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","path":"/usr/lib/homer_simpson.cf"})");
}
}  // namespace test_filepath
