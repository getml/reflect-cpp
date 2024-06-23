#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_generic {

TEST(json, test_generic) {
  auto bart = rfl::Generic::Object();
  bart["first_name"] = "Bart";
  bart["last_name"] = "Simpson";
  bart["age"] = 10;

  auto lisa = rfl::Generic::Object();
  lisa["first_name"] = "Lisa";
  lisa["last_name"] = "Simpson";
  lisa["age"] = 8;

  auto maggie = rfl::Generic::Object();
  maggie["first_name"] = "Lisa";
  maggie["last_name"] = "Simpson";
  maggie["age"] = 0;

  auto homer = rfl::Generic::Object();
  homer["first_name"] = "Lisa";
  homer["last_name"] = "Simpson";
  homer["age"] = 45;
  homer["children"] = rfl::Generic::Array({bart, lisa, maggie});

  write_and_read(
      homer,
      R"({"first_name":"Lisa","last_name":"Simpson","age":45,"children":[{"first_name":"Bart","last_name":"Simpson","age":10},{"first_name":"Lisa","last_name":"Simpson","age":8},{"first_name":"Lisa","last_name":"Simpson","age":0}]})");
}
}  // namespace test_generic
