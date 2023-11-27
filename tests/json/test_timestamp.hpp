#include <ctime>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_timestamp {

using TS = rfl::Timestamp<"%Y-%m-%d">;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  TS birthday;
};

void test() {
  std::cout << "test_timestamp" << std::endl;

  const auto result = TS::from_string("nonsense");

  if (result) {
    std::cout << "Failed: Expected an error, but got none." << std::endl;
    return;
  }

  const auto bart = Person{.first_name = "Bart", .birthday = "1987-04-19"};

  write_and_read(
      bart,
      R"({"firstName":"Bart","lastName":"Simpson","birthday":"1987-04-19"})");
}
}  // namespace test_timestamp
