#include <filesystem>
#include <string>

#include "write_and_read.hpp"
import rfl;
import rfl.json;


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

// Regression test for issue #421: a std::filesystem::path containing non-ASCII
// characters previously round-tripped through the locale-dependent
// path::string() / narrow path constructor and could throw or be mangled.
// (This file is UTF-8 encoded.)
TEST(json, test_filepath_non_ascii) {
  const auto homer =
      Person{.first_name = "Homer",
             .path = std::filesystem::path(u8"/home/中文/café.json")};

  write_and_read(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","path":"/home/中文/café.json"})");
}
}  // namespace test_filepath
