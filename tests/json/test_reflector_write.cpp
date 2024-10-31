#include <cassert>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_reflector_write {

struct Person {
  std::string first_name = "Homer";
  std::string last_name = "Simpson";
};

struct Parent : Person {
  std::vector<Person> children;
};

}  // namespace test_reflector_write

namespace rfl {
template <>
struct Reflector<test_reflector_write::Parent> {
  struct ReflType {
    std::string first_name;
    std::string last_name;
    std::vector<test_reflector_write::Person> children;
  };

  static ReflType from(const test_reflector_write::Parent& v) {
    return {v.first_name, v.last_name, v.children};
  }
};
}  // namespace rfl

namespace test_reflector_write {

TEST(json, test_reflector_write) {
  const auto homer = test_reflector_write::Parent{
      {"Homer", "Simpson"}, {{"Bart", "Simpson"}, {"Lisa", "Simpson"}}};

  write(
      homer,
      R"({"first_name":"Homer","last_name":"Simpson","children":[{"first_name":"Bart","last_name":"Simpson"},{"first_name":"Lisa","last_name":"Simpson"}]})");
}
}  // namespace test_reflector_write
