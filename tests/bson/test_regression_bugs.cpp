#include <gtest/gtest.h>

#include <bson/bson.h>

#include <cstdint>
#include <string>
#include <vector>

#include <rfl.hpp>
#include <rfl/bson.hpp>

// bson::Reader — float fields reject integer BSON values
// File: include/rfl/bson/Reader.hpp:127-132
// The floating-point branch checks `if (btype != BSON_TYPE_DOUBLE)` and
// returns an error, even though the error message claims int32/int64/date_time
// are valid types. The Writer stores all integers as BSON_TYPE_INT64
// (bson_append_int64). When reading those bytes into a struct with a double
// field, the Reader rejects the int64 value.
namespace test_bson_reader_int_as_float {

struct WithDouble {
  double value;
};

struct WithInt {
  int32_t value;
};

// Writer uses bson_append_int64 for all integral types, so even int32_t
// is stored as BSON_TYPE_INT64. The Reader's floating-point branch should
// accept BSON_TYPE_INT64 (and INT32) and convert to double.
TEST(bson_regression, read_int64_into_double_field) {
  const auto int_obj = WithInt{.value = 42};
  const auto bytes = rfl::bson::write(int_obj);

  const auto res = rfl::bson::read<WithDouble>(bytes);
  ASSERT_TRUE(res)
      << "Reading BSON int64 (written from int32_t) into a double field "
         "should succeed. Error: " << res.error().what();
  EXPECT_DOUBLE_EQ(res.value().value, 42.0)
      << "BSON int64 value 42 should be readable as double 42.0";
}

struct WithInt64Field {
  int64_t value;
};

TEST(bson_regression, read_large_int64_into_double_field) {
  const auto int64_obj = WithInt64Field{.value = 1000000};
  const auto bytes = rfl::bson::write(int64_obj);

  const auto res = rfl::bson::read<WithDouble>(bytes);
  ASSERT_TRUE(res)
      << "Reading BSON int64 into a double field should succeed. "
         "Error: " << res.error().what();
  EXPECT_DOUBLE_EQ(res.value().value, 1000000.0)
      << "BSON int64 value should be readable as double";
}

}  // namespace test_bson_reader_int_as_float
