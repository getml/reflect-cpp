#include <map>
#include <rfl.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_map {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::map<std::string, std::string> attributes;
};

TEST(boost_serialization, test_map) {
  const auto homer =
      Person{.first_name = "Homer",
             .attributes = {{"occupation", "Safety Inspector"},
                            {"town", "Springfield"}}};

  write_and_read(homer);
}
}  // namespace test_map
