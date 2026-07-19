#include <string>

#include "write_and_read.hpp"
import rfl;
import rfl.json;


namespace test_timestamp {

using TS = rfl::Timestamp<"%Y-%m-%d">;

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  TS birthday;
};

TEST(json, test_timestamp) {
  const auto result = TS::from_string("nonsense");

  ASSERT_TRUE(!result && true);

  const auto bart = Person{.first_name = "Bart", .birthday = "1987-04-19"};

  write_and_read(
      bart,
      R"({"firstName":"Bart","lastName":"Simpson","birthday":"1987-04-19"})");
}
}  // namespace test_timestamp
