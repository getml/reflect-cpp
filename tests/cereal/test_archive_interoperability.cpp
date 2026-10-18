#include <gtest/gtest.h>

#include "archive_reference.hpp"

#include <test_rfl/rfl.cereal.hpp>

namespace test_cereal_archive {
namespace {
const Record sample{-12345, 0xfedcba9876543210ULL, 1.25, true, "Cereal bridge"};

void expect_equal(const Record& _value) {
  EXPECT_EQ(_value.small, sample.small);
  EXPECT_EQ(_value.large, sample.large);
  EXPECT_EQ(_value.number, sample.number);
  EXPECT_EQ(_value.flag, sample.flag);
  EXPECT_EQ(_value.text, sample.text);
}
}  // namespace

TEST(cereal, archive_wire_compatibility) {
  const auto bytes = rfl::cereal::write(sample);
  EXPECT_EQ(bytes, reference_bytes(sample));
  expect_equal(reference_read(bytes));
  const auto result = rfl::cereal::read<Record>(reference_bytes(sample));
  ASSERT_TRUE(result);
  expect_equal(result.value());
}

TEST(cereal, existing_archive_overloads) {
  const auto bytes = with_output_archive(
      [](auto& archive) { rfl::cereal::write(sample, archive); });
  EXPECT_EQ(bytes, reference_bytes(sample));
  with_input_archive(bytes, [](auto& archive) {
    const auto result = rfl::cereal::read<Record>(archive);
    ASSERT_TRUE(result);
    expect_equal(result.value());
  });
}
}  // namespace test_cereal_archive
