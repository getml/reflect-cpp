#include <map>
#include <rfl.hpp>
#include <set>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_basic_types {

struct Person {
  std::string first_name;
  std::string last_name;
  int age;
};

TEST(boost_serialization, test_simple_struct) {
  const auto homer =
      Person{.first_name = "Homer", .last_name = "Simpson", .age = 45};
  write_and_read_with_json(homer);
}

struct Address {
  std::string street;
  std::string city;
  int zip;
};

struct PersonWithAddress {
  std::string first_name;
  Address address;
};

TEST(boost_serialization, test_nested_struct) {
  const auto homer =
      PersonWithAddress{.first_name = "Homer",
                        .address = Address{.street = "742 Evergreen Terrace",
                                           .city = "Springfield",
                                           .zip = 62704}};
  write_and_read_with_json(homer);
}

enum class Color { red, green, blue };

struct Circle {
  float radius;
  Color color;
};

TEST(boost_serialization, test_enum) {
  const auto circle = Circle{.radius = 2.0f, .color = Color::green};
  write_and_read_with_json(circle);
}

struct WithContainers {
  std::string name;
  std::vector<std::string> hobbies;
  std::map<std::string, std::string> attributes;
  std::set<int> lucky_numbers;
};

TEST(boost_serialization, test_containers) {
  const auto homer =
      WithContainers{.name = "Homer",
                     .hobbies = {"bowling", "eating", "sleeping"},
                     .attributes = {{"occupation", "Safety Inspector"},
                                    {"town", "Springfield"}},
                     .lucky_numbers = {7, 13, 42}};
  write_and_read_with_json(homer);
}

struct WithOptional {
  std::string first_name;
  std::optional<std::string> middle_name;
  std::optional<std::vector<WithOptional>> children;
};

TEST(boost_serialization, test_optional_fields) {
  const auto bart = WithOptional{.first_name = "Bart"};
  const auto homer = WithOptional{
      .first_name = "Homer", .children = std::vector<WithOptional>({bart})};
  write_and_read_with_json(homer);
}

}  // namespace test_basic_types
