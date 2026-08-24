#include <gtest/gtest.h>

#include <rfl.hpp>
#include <string>

#include "write_and_read.hpp"

#if __has_include(<expected>)
#include <expected>
#endif

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L

namespace test_expected {

TEST(generic, test_expected_success) {
  const std::expected<std::string, int> value = "hello";
  write_and_read(value);
}

TEST(generic, test_expected_error) {
  const std::expected<std::string, int> value = std::unexpected(42);
  write_and_read(value);
}

struct Person {
  std::string name;
  std::expected<int, std::string> age;
};

TEST(generic, test_expected_in_struct) {
  const Person homer = {.name = "Homer", .age = 42};
  write_and_read(homer);

  const Person maggie = {.name = "Maggie", .age = std::unexpected("too young")};
  write_and_read(maggie);
}

}  // namespace test_expected

#endif  // __cpp_lib_expected
