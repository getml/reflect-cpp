#include <string>
#include <vector>

#include <optional>

#include "write_and_read.hpp"

#include <test_rfl/rfl.avro.hpp>
#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


namespace test_optional_fields {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::optional<std::vector<Person>> children;
};

TEST(avro, test_optional_fields) {
  const auto bart = Person{.first_name = "Bart"};

  const auto lisa = Person{.first_name = "Lisa"};

  const auto maggie = Person{.first_name = "Maggie"};

  const auto homer =
      Person{.first_name = "Homer",
             .children = std::vector<Person>({bart, lisa, maggie})};

  write_and_read_with_json(homer);
}
}  // namespace test_optional_fields
