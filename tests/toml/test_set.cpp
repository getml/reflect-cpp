#include <string>

#include <memory>
#include <set>
#include <utility>

#include "write_and_read.hpp"

#include <test_rfl/rfl.toml.hpp>
#include <test_rfl/rfl.hpp>


namespace test_set {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::set<std::string>> children;
};

TEST(toml, test_set) { 
  auto children = std::make_unique<std::set<std::string>>(
      std::set<std::string>({"Bart", "Lisa", "Maggie"}));

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  write_and_read(homer);
}
}  // namespace test_set
