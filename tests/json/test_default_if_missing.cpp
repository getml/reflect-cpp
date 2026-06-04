#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

using namespace std::literals;

namespace test_default_if_missing {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::string town;
};

TEST(json, test_default_if_missing) {
  auto homer = rfl::json::read<Person, rfl::DefaultIfMissing>(
                   R"({"first_name":"Homer"})")
                   .value();

  EXPECT_EQ(homer.first_name, "Homer");
  EXPECT_EQ(homer.last_name, "Simpson");
  EXPECT_EQ(homer.town, "");
}

TEST(json, test_default_if_missing_malformed) {
  using Str = rfl::Validator<std::string, rfl::Size<rfl::Minimum<1>>>;
  struct Sample {
    Str str;
  };
  static constexpr auto data = R"({"str":"anything"})"sv;
  // Macros don't like commas
  auto wrapper = []() {
    return rfl::json::read<Sample, rfl::DefaultIfMissing>(data);
  };
  // We throw as the default string value does not satisfy the validator
  ASSERT_ANY_THROW(wrapper());
}
}  // namespace test_default_if_missing
