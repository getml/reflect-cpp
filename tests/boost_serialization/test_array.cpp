#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_array {

struct Person {
  std::string first_name;
  int age;
  std::vector<std::string> hobbies;
};

TEST(boost_serialization, test_array) {
  const auto homer = Person{
      .first_name = "Homer",
      .age = 45,
      .hobbies = std::vector<std::string>({"bowling", "eating", "sleeping"})};

  write_and_read_with_json(homer);
}
}  // namespace test_array
