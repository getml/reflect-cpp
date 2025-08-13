#include <rfl.hpp>
#include <rfl/cbor.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_no_optionals {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  rfl::Rename<"children", std::optional<std::vector<Person>>> children;
};

struct OptionalPerson {
    std::optional<Person> opt_person;
};

struct Empty{};

TEST(cbor, test_no_optionals) {
  auto empty_struct_cbor = rfl::cbor::write<rfl::NoOptionals>(Empty{});
  auto no_person_cbor = rfl::cbor::write<rfl::NoOptionals>(OptionalPerson{});

  EXPECT_NE(empty_struct_cbor.size(), no_person_cbor.size());
}
}  // namespace test_no_optionals
