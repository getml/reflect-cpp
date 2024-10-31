#include <cassert>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_reflector_read {

struct Person {
  std::string first_name = "Homer";
  std::string last_name = "Simpson";
};

struct Parent : Person {
  std::vector<Person> children;
};

}  // namespace test_reflector_read

namespace rfl {
template <>
struct Reflector<test_reflector_read::Parent> {
  struct ReflType {
    std::string first_name;
    std::string last_name;
    std::vector<test_reflector_read::Person> children;
  };
  static test_reflector_read::Parent to(const ReflType& v) noexcept {
    return {{v.first_name, v.last_name}, v.children};
  }
};
}  // namespace rfl

namespace test_reflector_read {

TEST(json, test_reflector_read) {
  const auto homer = test_reflector_read::Parent{
      {"Homer", "Simpson"}, {{"Bart", "Simpson"}, {"Lisa", "Simpson"}}};

  read(
      R"({"first_name":"Homer","last_name":"Simpson","children":[{"first_name":"Bart","last_name":"Simpson"},{"first_name":"Lisa","last_name":"Simpson"}]})",
      homer);
}
}  // namespace test_reflector_read
