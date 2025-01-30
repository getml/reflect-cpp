#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_variants_in_vectors {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::vector<std::variant<Person, std::string>> children;
};

TEST(capnproto, test_variants_in_vectors) {
  const std::variant<Person, std::string> bart = Person{.first_name = "Bart"};

  const std::variant<Person, std::string> lisa = Person{.first_name = "Lisa"};

  const std::variant<Person, std::string> maggie =
      Person{.first_name = "Maggie"};

  const auto homer =
      Person{.first_name = "Homer",
             .children = std::vector<std::variant<Person, std::string>>(
                 {bart, lisa, maggie, "Unknown"})};

  write_and_read(homer);
}
}  // namespace test_variants_in_vectors
