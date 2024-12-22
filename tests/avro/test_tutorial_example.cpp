#include <gtest/gtest.h>

#include <iostream>
#include <rfl.hpp>
#include <rfl/avro.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

/// The basic example from the Avro C tutorial.
namespace test_tutorial_example {

const char PERSON_SCHEMA[] =
    "{\"type\":\"record\",\
  \"name\":\"Person\",\
  \"fields\":[\
     {\"name\": \"ID\", \"type\": \"long\"},\
     {\"name\": \"First\", \"type\": \"string\"},\
     {\"name\": \"Last\", \"type\": \"string\"},\
     {\"name\": \"Phone\", \"type\": \"string\"},\
     {\"name\": \"Age\", \"type\": \"int\"}]}";

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
  const auto schema = rfl::avro::Schema<Person>::from_json(PERSON_SCHEMA);
  const auto serialized1 = rfl::avro::write(person, schema.value());
  const auto res = rfl::avro::read<Person>(serialized1, schema.value());
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().value().what();
  const auto serialized2 = rfl::avro::write(res.value(), schema.value());
  EXPECT_EQ(serialized1, serialized2);
}
}  // namespace test_tutorial_example
