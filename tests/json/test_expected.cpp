#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

#if __has_include(<expected>)
#include <expected>
#endif

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L

namespace test_expected {

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  std::vector<std::expected<int, std::string>> ages;
};

TEST(json, test_expected_success) {
  const std::expected<int, std::string> value = 42;
  write_and_read(value, "42");
}

TEST(json, test_expected_error) {
  const std::expected<int, std::string> value =
      std::unexpected("Something went wrong.");
  write_and_read(value, R"({"error":"Something went wrong."})");
}

TEST(json, test_expected_in_struct) {
  const Person homer = {.first_name = "Homer",
                        .ages = std::vector<std::expected<int, std::string>>(
                            {42, std::unexpected("unknown age")})};
  write_and_read(
      homer, R"({"firstName":"Homer","ages":[42,{"error":"unknown age"}]})");
}

TEST(json, test_expected_struct_value) {
  const std::expected<Person, std::string> value =
      Person{.first_name = "Bart",
             .ages = std::vector<std::expected<int, std::string>>({10})};
  write_and_read(value, R"({"firstName":"Bart","ages":[10]})");
}

}  // namespace test_expected

#endif  // __cpp_lib_expected
