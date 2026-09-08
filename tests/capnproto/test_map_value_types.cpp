#include <map>
#include <rfl.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_map_value_types {

struct PersonWithStringMap {
  rfl::Rename<"firstName", std::string> first_name;
  std::map<std::string, std::string> nicknames;
};

TEST(capnproto, test_map_with_string_values) {
  auto nicknames = std::map<std::string, std::string>();
  nicknames["Bart"] = "El Barto";
  nicknames["Lisa"] = "Lis";
  nicknames["Maggie"] = "Mags";

  const auto homer = PersonWithStringMap{.first_name = "Homer",
                                         .nicknames = std::move(nicknames)};

  write_and_read(homer);
}
}  // namespace test_map_value_types
