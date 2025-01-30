#include <gtest/gtest.h>

#include <iostream>
#include <rfl.hpp>
#include <rfl/avro.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

/// The basic example from the Avro C tutorial.
namespace test_tutorial_example_automated_schema {

struct Person {
  size_t ID;
  std::string First;
  std::string Last;
  std::string Phone;
  int Age;
};

TEST(avro, test_tutorial_example_automated_schema) {
  const auto person = Person{.ID = 1,
                             .First = "Randal",
                             .Last = "Graves",
                             .Phone = "(555) 123-5678",
                             .Age = 30};
  write_and_read(person);
}
}  // namespace test_tutorial_example_automated_schema
