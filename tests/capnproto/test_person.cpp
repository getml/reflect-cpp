#include <gtest/gtest.h>

#include <iostream>
#include <rfl.hpp>
#include <rfl/capnproto.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_tutorial_example {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::vector<Person> children;
};

TEST(capnproto, test_person) {
  const auto bart = Person{.first_name = "Bart"};

  const auto lisa = Person{.first_name = "Lisa"};

  const auto maggie = Person{.first_name = "Maggie"};

  const auto homer =
      Person{.first_name = "Homer",
             .children = std::vector<Person>({bart, lisa, maggie})};

  write_and_read(homer);
}
}  // namespace test_tutorial_example
