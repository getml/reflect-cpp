#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_optionals_in_vectors {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::vector<std::optional<Person>> children;
};

TEST(capnproto, test_optionals_in_vectors) {
  const std::optional<Person> bart = Person{.first_name = "Bart"};

  const std::optional<Person> lisa = Person{.first_name = "Lisa"};

  const std::optional<Person> maggie = Person{.first_name = "Maggie"};

  const auto homer = Person{.first_name = "Homer",
                            .children = std::vector<std::optional<Person>>(
                                {bart, lisa, maggie, std::nullopt})};

  write_and_read(homer);
}
}  // namespace test_optionals_in_vectors
