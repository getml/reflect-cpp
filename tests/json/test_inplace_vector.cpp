#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

#if __has_include(<inplace_vector>)
#include <inplace_vector>
#endif

#if defined(__cpp_lib_inplace_vector)

namespace test_inplace_vector {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::inplace_vector<std::string, 3> children;
};

TEST(json, test_inplace_vector) {
  const auto homer =
      Person{.first_name = "Homer", .children = {"Bart", "Lisa", "Maggie"}};

  write_and_read(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","children":["Bart","Lisa","Maggie"]})");
}

TEST(json, test_inplace_vector_overflow) {
  const auto res = rfl::json::read<Person>(
      R"({"firstName":"Homer","children":["Bart","Lisa","Maggie","Hugo"]})");
  EXPECT_FALSE(res && true);
}

}  // namespace test_inplace_vector

#endif  // __cpp_lib_inplace_vector
