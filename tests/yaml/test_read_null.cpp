#include <gtest/gtest.h>

#include <cmath>
#include <optional>
#include <rfl/yaml.hpp>
#include <string>

namespace test_read_null {

struct Person {
  std::string first_name;
  std::optional<std::string> last_name;
  std::optional<int> age;
  std::string city;
  float weight;
  double height;  
};

TEST(yaml, test_read_null) {
  const std::string yaml_str =
      "first_name: Homer\n"
      "last_name: null\n"
      "age: ~\n"
      "city: null\n"
      "weight: .nan\n"
      "height: .nan\n";

  const auto res = rfl::yaml::read<Person>(yaml_str);

  ASSERT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  EXPECT_EQ(res.value().first_name, "Homer");
  EXPECT_FALSE(res.value().last_name.has_value());
  EXPECT_FALSE(res.value().age.has_value());
  EXPECT_TRUE(std::isnan(res.value().weight));
  EXPECT_TRUE(std::isnan(res.value().height));
}

}  // namespace test_read_null
