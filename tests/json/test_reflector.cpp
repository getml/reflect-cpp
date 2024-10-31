#include <cassert>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_reflector {

struct Person {
  std::string first_name = "Homer";
  std::string last_name = "Simpson";
};

struct Parent : Person {
  std::vector<Person> children;
};

}  // namespace test_reflector

namespace rfl {
template <>
struct Reflector<test_reflector::Parent> {
  struct ReflType {
    std::string first_name;
    std::string last_name;
    std::vector<test_reflector::Person> children;
  };
  static test_reflector::Parent to(const ReflType& v) noexcept {
    return {{v.first_name, v.last_name}, v.children};
  }

  static ReflType from(const test_reflector::Parent& v) {
    return {v.first_name, v.last_name, v.children};
  }
};
}  // namespace rfl

namespace test_reflector {

TEST(json, test_reflector) {
  const auto homer = test_reflector::Parent{
      {"Homer", "Simpson"}, {{"Bart", "Simpson"}, {"Lisa", "Simpson"}}};

  write_and_read(
      homer,
      R"({"first_name":"Homer","last_name":"Simpson","children":[{"first_name":"Bart","last_name":"Simpson"},{"first_name":"Lisa","last_name":"Simpson"}]})");
}
}  // namespace test_reflector
