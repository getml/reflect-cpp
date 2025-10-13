#include <rfl.hpp>
#include <rfl/cbor.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_optional_fields {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  rfl::Rename<"children", std::optional<std::vector<Person>>> children;

  bool operator==(const Person& other) const {
    return first_name.get() == other.first_name.get() &&
           last_name.get() == other.last_name.get() &&
           children.get() == other.children.get();
  }
};

struct OptionalPerson {
    std::optional<Person> opt_person;
};

struct Empty{};

TEST(cbor, test_optional_no_fields) {
  auto empty_struct_cbor = rfl::cbor::write(Empty{});
  auto no_person_cbor = rfl::cbor::write(OptionalPerson{});

  EXPECT_EQ(empty_struct_cbor.size(), no_person_cbor.size());
}

TEST(cbor, test_optional_fields) {
  const auto bart = Person{.first_name = "Bart"};

  const auto lisa = Person{.first_name = "Lisa"};

  const auto maggie = Person{.first_name = "Maggie"};

  const auto homer =
      Person{.first_name = "Homer",
             .children = std::vector<Person>({bart, lisa, maggie})};

  const OptionalPerson homer_optional = {
    .opt_person = homer
  };

  const auto homer_optional_cbor = rfl::cbor::write(homer_optional);
  const auto res = rfl::cbor::read<OptionalPerson>(homer_optional_cbor);

  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                              << res.error().what();

  const auto actual_homer = res.value();

  EXPECT_TRUE(actual_homer.opt_person.has_value());
  EXPECT_EQ(*actual_homer.opt_person, homer);
}
}  // namespace test_optional_fields
