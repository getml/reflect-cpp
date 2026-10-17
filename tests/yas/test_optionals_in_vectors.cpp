#include <string>
#include <vector>

#include <optional>

#include "write_and_read.hpp"

#include <test_rfl/rfl.yas.hpp>
#include <test_rfl/rfl.hpp>


namespace test_optionals_in_vectors {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::vector<std::optional<Person>> children;
};

TEST(yas, test_optionals_in_vectors) {
  const std::optional<Person> bart = Person{.first_name = "Bart"};

  const std::optional<Person> lisa = Person{.first_name = "Lisa"};

  const std::optional<Person> maggie = Person{.first_name = "Maggie"};

  const auto homer = Person{.first_name = "Homer",
                            .children = std::vector<std::optional<Person>>(
                                {bart, lisa, maggie, std::nullopt})};

  write_and_read(homer);
}
}  // namespace test_optionals_in_vectors
