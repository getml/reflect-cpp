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
  std::vector<Person> children;
};

}  

namespace rfl {
template <>
struct Reflector<test_reflector::Person> {
  struct ReflType {
    std::string first_name;
    std::string last_name;
    std::vector<Person> children;
  };
  static Person to(const ReflType& v) noexcept {
    return {v.first_name, v.last_name, v.children};
  }

  static ReflType from(const Person& v) {
    return {v.first_name, v.last_name, v.children};
  }
};
}  // namespace rfl

namespace test_reflector {

TEST(json, test_reflector) {
  const auto homer =
      Person{"Homer", "Simpson", {{"Bart", "Simpson"}, {"Lisa", "Simpson"}}};

  write_and_read(
      homer,
      R"({"first_name":"Homer","last_name":"Simpson","children":[{"first_name":"Bart","last_name":"Simpson","children":[]},{"first_name":"Lisa","last_name":"Simpson","children":[]}]})");
}
}  // namespace test_reflector
