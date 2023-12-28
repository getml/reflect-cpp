#include "test_all_of.hpp"

#include <iostream>
#include <rfl/flexbuf.hpp>
#include <source_location>
#include <string>
#include <vector>

// Make sure things still compile when
// rfl.hpp is included after rfl/flexbuf.hpp.
#include <rfl.hpp>

#include "write_and_read.hpp"

namespace test_all_of {

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  using Age = rfl::Validator<unsigned int,
                             rfl::AllOf<rfl::Minimum<0>, rfl::Maximum<130>>>;

  struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name;
    rfl::Field<"age", Age> age;
  };

  const auto homer =
      Person{.first_name = "Homer", .last_name = "Simpson", .age = 45};

  write_and_read(homer);
}
}  // namespace test_all_of
