#include <gtest/gtest.h>

#include <rfl.hpp>
#include <rfl/avro.hpp>
#include <string>

/// The basic example from the Avro C tutorial.
namespace test_tutorial_example {

struct Person {
  size_t ID;
  std::string First;
  std::string Last;
  std::string Phone;
  int Age;
};

TEST(avro, test_tutorial_example) {
  const auto person = Person{.ID = 1,
                             .First = "Randal",
                             .Last = "Graves",
                             .Phone = "(555) 123-5678",
                             .Age = 30};
  const auto serialized1 = rfl::avro::write(person);
  const auto res = rfl::avro::read<Person>(serialized1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  const auto serialized2 = rfl::avro::write(res.value());
  EXPECT_EQ(serialized1, serialized2);
}
}  // namespace test_tutorial_example
