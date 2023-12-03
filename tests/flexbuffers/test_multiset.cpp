#include "test_multiset.hpp"

#include <iostream>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_multiset {

struct Person {
  rfl::Field<"firstName", std::string> first_name;
  rfl::Field<"lastName", std::string> last_name = "Simpson";
  rfl::Field<"children", std::unique_ptr<std::multiset<std::string>>> children =
      rfl::default_value;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  auto children = std::make_unique<std::multiset<std::string>>(
      std::multiset<std::string>({"Bart", "Lisa", "Maggie"}));

  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};

  write_and_read(homer);
}
}  // namespace test_multiset
