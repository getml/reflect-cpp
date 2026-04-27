#include <gtest/gtest.h>

#include <cmath>
#include <cstdint>
#include <optional>
#include <rfl/yaml.hpp>
#include <string>
#include <string_view>

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

TEST(yaml, test_null_for_required_field_fails) {
  const std::string yaml_str =
      "first_name: Homer\n"
      "last_name: null\n"
      "age: ~\n"
      "city: Springfield\n"
      "weight: null\n"
      "height: null\n";

  const auto res = rfl::yaml::read<Person>(yaml_str);
  EXPECT_FALSE(res.has_value());
}

// One struct per primitive type — each tested with a null value below.

struct IntHolder { int value; };
struct Int8Holder { int8_t value; };
struct Int16Holder { int16_t value; };
struct Int32Holder { int32_t value; };
struct Int64Holder { int64_t value; };
struct UInt8Holder { uint8_t value; };
struct UInt16Holder { uint16_t value; };
struct UInt32Holder { uint32_t value; };
struct UInt64Holder { uint64_t value; };
struct FloatHolder { float value; };
struct DoubleHolder { double value; };
struct BoolHolder { bool value; };

struct OptionalIntHolder { std::optional<int> value; };
struct OptionalStringHolder { std::optional<std::string> value; };

static constexpr std::string_view kNullYaml = "value: null\n";
static constexpr std::string_view kNaNYaml = "value: .nan\n";

TEST(yaml, test_null_for_int_fails) {
  EXPECT_FALSE(rfl::yaml::read<IntHolder>(kNullYaml).has_value());
}

TEST(yaml, test_null_for_int8_fails) {
  EXPECT_FALSE(rfl::yaml::read<Int8Holder>(kNullYaml).has_value());
}

TEST(yaml, test_null_for_int16_fails) {
  EXPECT_FALSE(rfl::yaml::read<Int16Holder>(kNullYaml).has_value());
}

TEST(yaml, test_null_for_int32_fails) {
  EXPECT_FALSE(rfl::yaml::read<Int32Holder>(kNullYaml).has_value());
}

TEST(yaml, test_null_for_int64_fails) {
  EXPECT_FALSE(rfl::yaml::read<Int64Holder>(kNullYaml).has_value());
}

TEST(yaml, test_null_for_uint8_fails) {
  EXPECT_FALSE(rfl::yaml::read<UInt8Holder>(kNullYaml).has_value());
}

TEST(yaml, test_null_for_uint16_fails) {
  EXPECT_FALSE(rfl::yaml::read<UInt16Holder>(kNullYaml).has_value());
}

TEST(yaml, test_null_for_uint32_fails) {
  EXPECT_FALSE(rfl::yaml::read<UInt32Holder>(kNullYaml).has_value());
}

TEST(yaml, test_null_for_uint64_fails) {
  EXPECT_FALSE(rfl::yaml::read<UInt64Holder>(kNullYaml).has_value());
}

TEST(yaml, test_null_for_float_fails) {
  EXPECT_FALSE(rfl::yaml::read<FloatHolder>(kNullYaml).has_value());
}

TEST(yaml, test_nan_for_float_succeeds) {
  EXPECT_TRUE(std::isnan(rfl::yaml::read<FloatHolder>(kNaNYaml).value().value));
}

TEST(yaml, test_null_for_double_fails) {
  EXPECT_FALSE(rfl::yaml::read<DoubleHolder>(kNullYaml).has_value());
}

TEST(yaml, test_nan_for_double_succeeds) {
  EXPECT_TRUE(std::isnan(rfl::yaml::read<DoubleHolder>(kNaNYaml).value().value));
}

TEST(yaml, test_null_for_bool_fails) {
  EXPECT_FALSE(rfl::yaml::read<BoolHolder>(kNullYaml).has_value());
}

TEST(yaml, test_null_for_optional_int_is_nullopt) {
  const auto res = rfl::yaml::read<OptionalIntHolder>(kNullYaml);
  ASSERT_TRUE(res.has_value());
  EXPECT_FALSE(res.value().value.has_value());
}

TEST(yaml, test_null_for_optional_string_is_nullopt) {
  const auto res = rfl::yaml::read<OptionalStringHolder>(kNullYaml);
  ASSERT_TRUE(res.has_value());
  EXPECT_FALSE(res.value().value.has_value());
}

}  // namespace test_read_null
