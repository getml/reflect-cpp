#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_default_if_missing {

struct Person {
  std::string name;
  std::vector<std::string> children;
};

TEST(xml, test_default_if_missing) {
  const auto homer =
      Person{.name = "Homer",
             .children = std::vector<std::string>({"Bart", "Lisa", "Maggie"})};

  write_and_read<"Person", rfl::DefaultIfMissing>(homer);
}
}  // namespace test_default_if_missing
