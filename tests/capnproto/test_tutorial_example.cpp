#include <gtest/gtest.h>

#include <iostream>
#include <rfl.hpp>
#include <rfl/capnproto.hpp>
#include <string>
#include <vector>

// #include "write_and_read.hpp"

/// The basic example from the capnproto C tutorial.
namespace test_tutorial_example {

const std::string ADDRESS_BOOK_SCHEMA = R"(
@0xb07899ba441fd7ec;

struct Person {
  id @0 :UInt32;
  name @1 :Text;
  email @2 :Text;
  phones @3 :List(PhoneNumber);

  struct PhoneNumber {
    number @0 :Text;
    type @1 :Type;

    enum Type {
      mobile @0;
      home @1;
      work @2;
    }
  }

  employment :union {
    unemployed @4 :Void;
    employer @5 :Text;
    school @6 :Text;
    selfEmployed @7 :Void;
    # We assume that a person is only one of these.
  }
}

struct AddressBook {
  people @0 :List(Person);
}
)";

enum class Type { mobile, home, work };

struct PhoneNumber {
  std::string number;
  std::string email;
  Type type;
};

struct Person {
  uint32_t id;
  std::string name;
  std::string email;
  // TODO: Employment.
};

struct AddressBook {
  std::vector<Person> people;
};

TEST(capnproto, test_tutorial_example) {
  rfl::capnproto::Schema<Person>::from_string(ADDRESS_BOOK_SCHEMA).value();
}
}  // namespace test_tutorial_example
