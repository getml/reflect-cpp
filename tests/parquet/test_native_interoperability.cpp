#include <gtest/gtest.h>

#include <cstdint>
#include <optional>
#include <string>
#include <test_rfl/rfl.parquet.hpp>
#include <vector>

#include "native_reference.hpp"

namespace test_parquet_bridge {
struct Record {
  int32_t count;
  std::optional<int64_t> maybe;
  double score;
  rfl::Timestamp<"%Y-%m-%d %H:%M:%S"> when;
  std::string name;
  rfl::Bytestring bytes;
};
TEST(parquet, native_interoperability) {
  const auto records =
      rfl::parquet::read<std::vector<Record>>(reference_bytes());
  ASSERT_TRUE(records) << records.error().what();
  ASSERT_EQ(records->size(), 2U);
  EXPECT_EQ((*records)[0].count, 42);
  EXPECT_FALSE((*records)[0].maybe);
  EXPECT_EQ((*records)[1].maybe, 17);
  EXPECT_EQ((*records)[1].score, 2.5);
  EXPECT_EQ((*records)[1].when.to_time_t(), 1704067201);
  EXPECT_EQ((*records)[0].name, "native 世界");
  const auto bytes =
      rfl::parquet::write(*records, rfl::parquet::Settings{.chunksize = 1});
  EXPECT_TRUE(reference_check(bytes));
}
}  // namespace test_parquet_bridge

namespace test_parquet_bridge {
TEST(parquet, chunked_nullable_bridge) {
  std::vector<Record> expected;
  for (int32_t i = 0; i < 513; ++i) {
    expected.push_back(
        Record{i - 256, i % 3 ? std::optional<int64_t>(i * 1000) : std::nullopt,
               i * 0.25, rfl::Timestamp<"%Y-%m-%d %H:%M:%S">(1704067200 + i),
               std::string(100, static_cast<char>('a' + i % 26)),
               rfl::Bytestring{std::byte{0}, std::byte{0xff}}});
  }
  const auto bytes =
      rfl::parquet::write(expected, rfl::parquet::Settings{.chunksize = 128});
  const auto actual = rfl::parquet::read<std::vector<Record>>(bytes);
  ASSERT_TRUE(actual) << actual.error().what();
  ASSERT_EQ(actual->size(), expected.size());
  for (size_t i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(actual->at(i).count, expected[i].count);
    EXPECT_EQ(actual->at(i).maybe, expected[i].maybe);
    EXPECT_EQ(actual->at(i).score, expected[i].score);
    EXPECT_EQ(actual->at(i).when.to_time_t(), expected[i].when.to_time_t());
    EXPECT_EQ(actual->at(i).name, expected[i].name);
    EXPECT_EQ(actual->at(i).bytes, expected[i].bytes);
  }
}
}  // namespace test_parquet_bridge
